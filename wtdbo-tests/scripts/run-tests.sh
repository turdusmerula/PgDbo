#!/bin/bash

xpl_path=$(cd -P -- "$(dirname -- "$0")" && pwd -P)
root_path=$(echo ${xpl_path} | sed "s#\(.*\)/wtdbo-tests/.*#\1#g")
cd $xpl_path

conf_path=${xpl_path}/../conf
bin_path=${root_path}/Build/bin

help=false
filtargs=""
while [[ $# > 0 ]]
do
	key="$1"

	if [[ "_${key}" == "_--help" ]]; 
	then
	    help=true
		filtargs="$filtargs $key"
	elif [[ "_${key}" == "_--ddd" ]]; 
	then
	    ddd=true
	elif [[ "_${key}" == "_--profiler=LeakSanitizer" ]]; 
	then
	    lsan=true
	elif [[ "_${key}" == "_--profiler=AddressSanitizer" ]]; 
	then
	    asan=true
	elif [[ "_${key}" == "_--profiler=tcmalloc" ]]; 
	then
	    tcmalloc=true
	elif [[ "_${key}" == "_--profiler=valgrind" ]]; 
	then
		echo poupou
	    valgrind=true
	elif [[ "_${key}" == "_--profiler=callgrind" ]]; 
	then
	    callgrind=true
	elif [[ "_${key}" == "_--profiler=ftrace" ]]; 
	then
	    ftrace=true
	else
		filtargs="$filtargs $key"
	fi		
	shift # past argument or value
done

if [ $help == false ]
then
	# create integration database
	echo " --> Create integration database"
	sudo -iu postgres psql -f ${conf_path}/integration.sql
	sudo -iu postgres psql bournioutests -f ${conf_path}/postgis.sql
fi

echo "args: $filtargs" 

# run tests
echo " --> run tests"
if [ $ddd ]
then
	ddd --debugger "gdb -x ${conf_path}/gdb.ini --args ${bin_path}/wtdbo-tests $filtargs"
elif [ $asan ]
then
	echo "[ Run tests with AddressSanitizer ]"
	export ASAN_OPTIONS="detect_leaks=1 symbolize=1 fast_unwind_on_malloc=0 external_symbolizer_path=/usr/bin/llvm-symbolizer-3.6" 
	( ${bin_path}/wtdbo-tests $filtargs 2>&1 ) | /opt/asan_symbolize.py 
elif [ $lsan ]
then
	echo "[ Run tests with LeakSanitizer ]"
	export LSAN_OPTIONS="detect_leaks=1 symbolize=1 fast_unwind_on_malloc=0 external_symbolizer_path=/usr/bin/llvm-symbolizer-3.6" 
	( ${bin_path}/wtdbo-tests $filtargs 2>&1 ) | /opt/asan_symbolize.py 
elif [ $tcmalloc ]
then
	echo "[ Run tests with tcmalloc ]"
	rm /tmp/*.heap
	#	export HEAPPROFILE="/tmp/testHeapProfile"
	
	# deactivate internal pools of the STL that create false leaks
	export GLIBCPP_FORCE_NEW=1
	export GLIBCXX_FORCE_NEW=1
	
	${bin_path}/wtdbo-tests $filtargs

	/opt/gperftools/src/pprof --text --stacks ${bin_path}/wtdbo-tests /tmp/testHeapProfile.0003.heap	
	
	echo "---- diff between /tmp/testHeapProfile.0001.heap and /tmp/testHeapProfile.0002.heap"
	/opt/gperftools/src/pprof --text --stacks --base=/tmp/testHeapProfile.0001.heap ${bin_path}/wtdbo-tests /tmp/testHeapProfile.0002.heap	
elif [ $valgrind ]
then
	echo "[ Run tests with valgrind ]"
	
elif [ $callgrind ]
then
	echo "[ Run tests with callgrind ]"
	valgrind --tool=callgrind ${bin_path}/wtdbo-tests $filtargs
elif [ $ftrace ]
then
	echo "[ Run tests with ftrace ]"
	LD_PRELOAD=/opt/ftrace/Build/lib/libftracelib.so FTRACE=${conf_path}/ftrace.fcf ${bin_path}/wtdbo-tests $filtargs
else
	${bin_path}/wtdbo-tests $filtargs
fi
