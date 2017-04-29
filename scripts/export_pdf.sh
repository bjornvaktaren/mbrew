if [[ ! $# = 1 ]]
then
    echo "Usage:"
    echo "${0} RECIPE"
else
    recipe=$1
    tmp_dir=export_pdf_tmp
    mkdir -p $tmp_dir
    name=${recipe/\/*\//}
    name=${name/.*/}
    latex=${name/.*/}.tex
    pdf=${name/.*/}.pdf
    mbrew=./mbrew
    LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD/../lib/ \
		   eval $mbrew $recipe --latex $tmp_dir/$latex && \
	cd $tmp_dir && \
	lualatex $latex && \
	lualatex $latex && \
	mv $pdf ../ && \
	cd ../ && \
	rm -r $tmp_dir
fi


    

