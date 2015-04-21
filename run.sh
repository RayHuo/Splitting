#! /bin/bash

# cd IO/pealGraph/
# for file in `ls`
# do
# 	if[ ${file} != "hc.lp" ]; then
# 		echo > ${file}
# 	fi
# done

# ./cpgm

# cd IO/output/
# rm -rf ./*

# cd ..

cd IO/
> count_out.txt

cd ./input/
fileNames=(`ls`)
cd ../pealGraph/cuts/
cutNames=(`ls`)
inpre="IO/input/"
cutpre="IO/pealGraph/cuts/"

cd ../../../

num=${#fileNames[@]}

for ((i=0; i<num; i++));
do
	argus=${inpre}${fileNames[i]}
    cut=${cutpre}${cutNames[i]}
    # ii=$((i+3))
    echo ${argus}" "${cut}  # " "${ii}
    ./splitting ${argus} ${cut} 
done


# cat the results
summary='splittingSummanry'
> ${summary}
outputPath='IO/output/'
for outFile in `ls IO/output/`
do
    cat ${outputPath}${outFile} >> ${summary}
done

# run clasp and cat the results
claspSummary='claspSummary'
> ${claspSummary}
claspOutPath="IO/claspOutput"
claspInPath="IO/input/"
for claspFile in `ls IO/input`
do
    echo ${claspFile} >> ${claspSummary}
    gringo ${claspInPath}${claspFile} | clasp 1 | grep 'Time' >> ${claspSummary}
    echo "" >> ${claspSummary}
done
