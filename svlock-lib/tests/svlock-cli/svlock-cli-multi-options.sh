#!/bin/sh

# basic strategy here is to run the osv-query program with various XML
# configuration files specified.  The output will be saved to
# a similarly named file and compared with another similarly named file
# that contains the expected output for the configuration.
#
# if the osv-query program returns an error status or the diff prorgram
# returns an error status, we'll set the passed environment variable to
# false.  The last thing the script does is run $passed so that will run
# either true or false to indicate success or failure of the set of
# programs.
#
# Code will also save the diff output in a conspicuous file to make it
# a little easier to diagnose any failures.

ignore_stderr="2>/dev/null"	# hide stderr output (if still set)
# ignore_stderr=""		# leave stderr alone 

files=""
files="$files access-1s"
files="$files access-2s"
files="$files PlatformConfig_hsx-4s"

passed=true

runit()
{
	base=$1
	act_suffix=$2
	exp_suffix=$3
	shift 3
	args=$*

	# set variables for the current test/config
	actual=actual/$base.$act_suffix.actual
	expected=expected/$base.$exp_suffix.expected
	config=../sysconfig/cfg-files/$base.xml
	diff=FAILURES/FAIL.$base.$act_suffix.diff
	# A few issues we need to worry about here:
	#	Order of output is undefined; need to sort it so we can
	#		compare against a reference result
	#	Need to check the exit status of osv-query
	# if we run "osv-query <stuff> | sort" or some variation, we'll get
	# the exit status of sort.
	# Solution is to run osv-query in a subshell that also reports the
	# exit status, and then sort the output which includes the line with
	# the exit status.
	# Also ran into problem with 'c' sorting after 'D' vs. before
	# essentially ASCII vs. alphabetic.  So set the locale (LC_ALL) so
	# we'll sort based on ASCII.
	cmd=`printf '( svlock-cli -f $config $args $ignore_stderr; echo exit status $? ) | LC_ALL=C sort >$actual\n'`

	mkdir -p actual FAILURES
	rm -f $diff $actual

	echo run with $config $args
	# need eval instead of plain "$cmd" so that the stderr redirection
	# happens if it is setup
	# echo $cmd
	eval $cmd
	status=$?
	if [ $status -ne 0 ]
	then
		passed=false
		printf "FAIL:  \"%s\" exited with failure status %d\n" \
			"$cmd" $status
	fi

	cmd="diff --strip-trailing-cr -u $expected $actual"
	if ! $cmd 
	then
		passed=false
		$cmd >$diff
	fi
}


for f in $files
do
	runit $f -c core -c
	runit $f -l list -l
	runit $f -t thread -t
	runit $f -s sock -s
	runit $f -m mem -m
	runit $f -M count -M
	runit $f -L domain -L

	runit $f -s-t sock-thread -s -t
	runit $f -t-l thread-list -t -l
	runit $f -cs core-sock -cs
	runit $f -tc core-thread -tc
	runit $f -ML mmconfig -ML

	runit $f long-c core --cpu-count
	runit $f long-l list --list-threads-sct
	runit $f long-s sock --socket-count
	runit $f long-m mem --list-memory
	runit $f long-t thread --thread-count
	runit $f long-M count --mmconfig-count
	runit $f long-L domain --list-mmconfig
	

	runit $f long-cs core-sock --cpu-count --socket-count
	runit $f long-st sock-thread --socket-count --thread-count
	runit $f long-tc core-thread --thread-count --cpu-count
	runit $f long-tl thread-list --thread-count --list-threads-sct
	runit $f long-ML mmconfig --mmconfig-count --list-mmconfig
done

echo passed is $passed
$passed
passed=true
