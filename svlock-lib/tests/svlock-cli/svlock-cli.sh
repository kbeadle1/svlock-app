#!/bin/sh

# basic strategy here is to run the osv-query program with various XML
# configuration files specified.  The output will be saved to
# a similarly named file and compared with another similarly named file
# that contains the expected output for the configuration.
#
# The osv-query program will be run with options to produce as much output
# as possible (exercise every option in each run).  Since the output is not
# intended to be in any specific order it is run through sort to produce
# a deterministic result.
#
# if the osv-query program returns an error status or the diff prorgram
# returns an error status, we'll set the passed environment variable to
# false.  the last thing the script does is run $passed so that will run
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
files="$files interleaved-memory"

passed=true

for f in $files
do
	# set variables for the current test/config
	actual=actual/$f.actual
	expected=expected/$f.expected
	config=../sysconfig/cfg-files/$f.xml
	diff=FAILURES/FAIL.$f.diff
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
	cmd=`printf '( svlock-cli -f $config -sctlpmML $ignore_stderr; echo exit status $? ) | LC_ALL=C sort >$actual'`

	rm -f $diff $actual
	mkdir -p actual FAILURES

	echo run with $f

	# need eval instead of plain "$cmd" so that the stderr redirection
	# happens if it is setup
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
done

echo passed is $passed
$passed
passed=true
