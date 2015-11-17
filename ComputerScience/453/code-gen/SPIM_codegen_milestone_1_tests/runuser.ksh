#!/bin/ksh -x
#
# Run Programming Assignment for User {make, tests}
# (TO BE EXECUTED FROM ASSIGNMENT DIRECTORY)

course_directory=/home/cs453/fall11
ADMINDIR=${course_directory}/ADMIN
ScriptDir=${ADMINDIR}/GradingScripts
TurninDir=/local/cs453/assg4ms1/

# This is where the test cases are found
#
test_directory=${course_directory}/TestCases/SPIM_codegen_milestone_1_tests

# This is where all the results will be placed
#
Assg=Assg4Milestone1
RESULTDIR=${ADMINDIR}/${Assg}
allout=${RESULTDIR}/results.out

binary=compile
out=Results
tmpout=tmp.results
failed=fail.tmp.out
score_out=Score
fail_out=Fails
#
maxscore=100
failpenalty=4
#
dailylatepenalty=10
num_days_late=0
#
  if test -d ${TurninDir}/late${num_days_late}/$1
  then
    cd ${TurninDir}/late${num_days_late}/$1
	/bin/rm $tmpout
	/bin/rm $failed
    echo $1
    echo "User: $1" > $out
    echo "" >> $out
    echo "" > $fail_out
    echo "" > $score_out
    echo "" >> $out
    echo "		       CSc 453: ASSIGNMENT:  $binary" >> $out
    echo "" >> $out
    egrep $1^ ${SCRIPT_HOME}/classlist_names >> $out
#    egrep $1^ $course_directory/admin/classlist_names >> $out
    echo "	USER:  $1		ASSIGNMENT RESULTS/COMMENTS" >> $out
    echo "" >> $out
    echo "Maximum for this assignment: $maxscore" >> $out
   ## echo "Your score:                  " >> $out
   # echo "" >> $out

#   Look for Makefile
    if test -f Makefile -o -f makefile
    then
 	make clean
	/bin/rm -f $binary 1> /dev/null 2>&1 
#       Make executable
        make $binary 1>make.out 2>&1
	if test -s make.out
	then
	    echo "" >> $tmpout
	    echo "MAKE OUTPUT  (upto 20 lines)" >> $tmpout
	    echo "" >> $tmpout
	    head -20 make.out >> $tmpout
	fi
	if test -f $binary 
	then
############################### LIMITS ##############################
 	    ulimit -c 0
	    ulimit -t 20
	    ulimit -f 1000
#####################################################################

#	    PERFORM TEST CASES ****************************************
	    echo "LEGAL INPUT: " >> $tmpout
            total_norm=0
	    echo "I. TESTING CORRECTNESS: " >> $tmpout
	    for n in 01 02 03 04 05 06 07 08 09 10\
		     11 12 13 14 15 16 17 18 19 20\
		     21 22 23 24 25 26 27
	    do
	      # BEGIN THIS TEST
	      echo "TEST{$n}"
              echo "" >> $tmpout
	      echo "TEST ${n}:"
              echo "   $binary < $test_directory/test${n}.c > t${n}.s" >> $tmpout

	      /bin/rm -f t${n}.s 1> /dev/null 2>&1 
	      /bin/rm -f test${n}.out 1> /dev/null 2>&1 

	      ./$binary < $test_directory/test${n}.c 1>t${n}.s 2>&1

	      echo "   spim -file t${n}.s" >> $tmpout
	      spim -file t${n}.s > test${n}.out 2>&1
	      echo "Exit Status = $?" >> $tmpout

	      if test -s test${n}.out
	      then
	          # Remove header
		  grep -v SPIM test${n}.out | grep -v Copyright | grep -v "All Rights Reserved" | grep -v "README" | grep -v "Loaded:" > temp
		  mv temp test${n}.out
   
	          diff test${n}.out $test_directory/out${n} > test${n}.diff
 	          if test -s test${n}.diff
	          then
                    head test${n}.diff >> $tmpout
                    echo >> $failed "test${n}"
	          else
                    total_norm=$(($total_norm +1))
                    echo "Passed" >> $tmpout
	          fi
	      	
	      else
                  echo >> $failed "test${n}"	     
 	      fi	

########################
	      # END OF THIS TEST
	    done
	    echo ""
            echo "" >> $tmpout
  
	    failed_norm=$((27-$total_norm))        	            
    	else
            echo "" >> $out
            echo "EXECUTABLE $binary NOT FOUND" >> $out
	    ((maxscore = maxscore - maxscore))
	    failed_norm=27
	fi
    else
        echo "" >> $out
        echo "NEITHER Makefile NOR makefile WAS FOUND" >> $out
	((maxscore = maxscore - maxscore))	
	failed_norm=27
    fi

    echo "Passed Test Cases  = $total_norm" >> $out            
    echo "Failed Test Cases  = $failed_norm" >> $out

    echo "FAILED TESTS:" >> $out
    #
    if test -s $failed
    then
        cat $failed >> $out
        /bin/cat $failed >> $fail_out
    else
        echo "  none" >> $out
    fi
    latepenalty=`echo "scale=2; $num_days_late * $dailylatepenalty" | bc`
    penalty=`echo "scale=2; $failed_norm * $failpenalty" | bc`
    totalpenalty=`echo "scale=2; $latepenalty + $penalty" | bc`
    score=`echo "$maxscore:$totalpenalty" | awk 'BEGIN { FS=":";} {if ($1 < $2) print 0; else print $1-$2;}'`
#
    echo "Total no. of failed tests = $failed_norm" >> $out
    echo "Penalty: $failed_norm * $failpenalty points/test = $penalty" >> $out
    echo "Late Penalty: $num_days_late days late @ $dailylatepenalty points/day = $latepenalty" >> $out
    echo "Your score: $maxscore - $penalty [fail] - $latepenalty [late] = $score" >> $out
    echo "" >> $out
    #
    cat $tmpout >> $out
    #   Copy results to mondo results file
    myname=`echo ${PWD} | awk -f ${ScriptDir}/get_cwd.awk`
    cp ${out} ${RESULTDIR}/${myname}.result

    cat $out /$course_directory/ADMIN/GradingScripts/newpage >> $allout
    echo "---END---" >> $allout
    #   Remove unnecessary files
    /bin/rm -f *.o *.out *.diff

    #   Leave this student's directory
    cd ..
  fi
