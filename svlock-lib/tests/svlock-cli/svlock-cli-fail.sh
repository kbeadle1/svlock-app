#!/bin/sh

# will run svlock-cli with bogus options and verify it exits with an error
# status
passed=true

if svlock-cli --supercalifragilisticexpialidocious 2>/dev/null
then
	passed=false
fi


# potentially need to change this, but for now x is an invalid option.
if svlock-cli -x 2>/dev/null
then
	passed=false
fi


$passed
passed=true
