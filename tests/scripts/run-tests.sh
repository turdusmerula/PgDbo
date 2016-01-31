#!/bin/bash

xpl_path=$(cd -P -- "$(dirname -- "$0")" && pwd -P)
root_path=$(echo ${xpl_path} | sed "s#\(.*\)/commons/.*#\1#g")
cd $xpl_path

conf_path=${xpl_path}/../conf
bin_path=${root_path}/Build/bin

help=false

args=$*

if [[ " ${args[@]} " =~ " --help " ]]; then
    help=true
fi

if [[ " ${args[@]} " =~ " --ddd " ]]; then
    ddd=true
fi

if [[ " ${args[@]} " =~ " --lsan " ]]; then
    lsan=true
fi

if [[ " ${args[@]} " =~ " --tcmalloc " ]]; then
    tcmalloc=true
fi

if [ $help == false ]
then
	# create integration database
	echo " --> Create integration database"
	sudo -iu postgres psql -f ${conf_path}/integration.sql
	sudo -iu postgres psql bournioutests -f ${conf_path}/postgis.sql
fi

# run tests
echo " --> run tests"
if [ $ddd ]
then
	ddd --debugger "gdb -x ${conf_path}/gdb.ini --args ${bin_path}/dbo-tests $*"
elif [ $lsan ]
then
	export LSAN_OPTIONS="detect_leaks=1 symbolize=1 fast_unwind_on_malloc=0 external_symbolizer_path=/usr/bin/llvm-symbolizer-3.6" 
	( ${bin_path}/dbo-tests $* 2>&1 ) | /opt/asan_symbolize.py 
elif [ $tcmalloc ]
then
	rm /tmp/*.heap
	#	export HEAPPROFILE="/tmp/testHeapProfile"
	
	# deactivate internal pools of the STL that create false leaks
	export GLIBCPP_FORCE_NEW=1
	export GLIBCXX_FORCE_NEW=1
	
	${bin_path}/commons-lib-tests $*

	/opt/gperftools/src/pprof --text --stacks ${bin_path}/dbo-tests /tmp/testHeapProfile.0003.heap	
	
	echo "---- diff between /tmp/testHeapProfile.0001.heap and /tmp/testHeapProfile.0002.heap"
	/opt/gperftools/src/pprof --text --stacks --base=/tmp/testHeapProfile.0001.heap ${bin_path}/dbo-tests /tmp/testHeapProfile.0002.heap	
else
	${bin_path}/dbo-tests $*
fi
