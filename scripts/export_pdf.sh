#!/bin/bash
if [[ ! $# = 1 ]] && [[ ! $# = 2 ]]
then
    echo "Usage:"
    echo "${0} RECIPE [PDF]"
else
    recipe=$1
    tmp_dir=export_pdf_tmp
    mkdir -p $tmp_dir
    echo "mkdir -p ${tmp_dir}"
    echo $recipe
    # extract recipe name by stripping path
    name=${recipe##*/}
    echo $name
    # also strip extension
    name=${name/.*/}
    echo $name
    latex=${name}.tex
    echo $latex
    pdf=${name}.pdf
    pdf_out=$pdf
    if [[ $# = 2 ]]
    then
	pdf_out=$2
    fi
    echo $pdf
    mbrew=./mbrew
    LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD/../lib/ \
		   eval $mbrew $recipe --latex $tmp_dir/$latex && \
	cd $tmp_dir && \
	lualatex $latex && \
	lualatex $latex && \
	mv $pdf ../$pdf_out && \
	cd ../ && \
	rm -r $tmp_dir
    echo "rm -r ${tmp_dir}"
fi


    

