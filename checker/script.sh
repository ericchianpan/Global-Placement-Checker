#!/bin/bash

big2_parameters=("400 400" "330 500" "415 405")
big3_parameters=("540 540" "545 550" "425 720")

cd "../PA03"
for dir in *;
do
    cd "$dir"
    echo "cd " $dir
    cp ../../benchmarks/aoi_big2.blif ../../benchmarks/aoi_big3.blif . 
    if [ -f "result.csv" ]; then
        rm result.csv
    fi
    if [ -f "timeresult" ]; then
        rm timeresult 
    fi
    for i in $(seq 0 `expr ${#big2_parameters[@]} - 1`); do
        item=(${big2_parameters[$i]})
        core_width=${item[0]}
        core_height=${item[1]}
        echo "execute place aoi_big2 with core width and height is $core_width and $core_height"
        STARTTIME=`date +%s.%N`
        timeout 1800s ./place aoi_big2.blif $core_width $core_height
        ENDTIME=`date +%s.%N`
        TIMEDIFF=`echo "$ENDTIME - $STARTTIME" | bc | awk -F"." '{print $1"."substr($2,1,3)}'`
        echo "Time diff is: $TIMEDIFF" >> timeresult
        ./../../checker/checker aoi_big2.blif aoi_big2.pl $core_width $core_height
        if [ -f "aoi_big2.pl" ]; then
            rm aoi_big2.pl 
        fi
    done
    for i in $(seq 0 `expr ${#big3_parameters[@]} - 1`); do
        item=(${big3_parameters[$i]})
        core_width=${item[0]}
        core_height=${item[1]}
        echo "execute place aoi_big3 with core width and height is $core_width and $core_height"
        STARTTIME=`date +%s.%N`
        timeout 1800s ./place aoi_big3.blif $core_width $core_height
        ENDTIME=`date +%s.%N`
        TIMEDIFF=`echo "$ENDTIME - $STARTTIME" | bc | awk -F"." '{print $1"."substr($2,1,3)}'`
        echo "Time diff is: $TIMEDIFF" >> timeresult
        ./../../checker/checker aoi_big3.blif aoi_big3.pl $core_width $core_height
        if [ -f "aoi_big3.pl" ]; then
            rm aoi_big3.pl 
        fi
    done
    cd ..
done
