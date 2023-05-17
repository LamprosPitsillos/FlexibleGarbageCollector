# #!/bin/bash
# CC="g++"
# CFLAGS=" -g -c -Wall --std=c++20 -fno-optimize-sibling-calls -fno-omit-frame-pointer -Og "
#
# for test in *.o;do
#     echo "${CC} ${CFLAGS} ${test} -o ${test%.*}"
# done
#
#!/bin/sh
CC="g++"
CFLAGS=" -g -Wall --std=c++20 -fno-optimize-sibling-calls -fno-omit-frame-pointer -Og "

LOGPATH="../Data/Logs"
ITEMS=(255 2047 8191 16384 32768)

if [ $# -eq 0 ]; then
    echo "No arguments provided. Please specify 'build' ,'bench <iterations>', 'test."
    exit 1
fi


case "$1" in
    build)
        cd .. && make -j8 test && cd -;

        OBJS=""
        echo "Collecting obj files..."
        for objs in ../Build/*.o; do
            OBJS+=" ${objs}"
        done

        if [ -z "$OBJS" ];then 
            echo "No OBJS found."
            exit 1
        fi

        for test in *.o; do
            echo "::::::::> Building '${test%.*}'..."
            ${CC} ${CFLAGS} ${OBJS} ${test} -o ${test%.*}
            # echo "${CC} ${CFLAGS} ${OBJS} ${test} -o ${test%.*}"
        done
        for bench in ../Benchmarks/*.o; do
            echo "::::::::> Building '${bench%.*}'..."
            ${CC} ${CFLAGS} ${OBJS} ${bench} -o ../Benchmarks/${bench%.*}
            # echo "${CC} ${CFLAGS} ${OBJS} ${test} -o ${test%.*}"
        done
        ;;

    clean)
        echo "removed for now ..."
        # for test in *.o; do
            # echo "Cleaning '${test%.*}'..."
            # rm ${test%.*}
        # done
        ;;
    test)
        for test in *.o; do
            executable="${test%.*}"
            if [ -x "$executable" ]; then
                path_to_csvs="${LOGPATH}/${executable}"
                echo "::::> Running ${executable} with all confs..."
                mkdir -p ${path_to_csvs}
                for conf in ./gc_configs/*.json; do
                    csv_name=$(basename ${conf} .json)
                    echo "::::::::> ${conf} "
                    ./${executable} ${conf} > ${path_to_csvs}/${executable}_${csv_name}.csv
                done
                
            else
                echo "::::> Executable ${executable} not found."
            fi
        done
        ;;
    bench)

if [ $# -ne 2 ]; then
    echo "Give iteration number 'bench <iterations>'"
    exit 1
fi
        for bench in ../Benchmarks/*.o; do
            executable="${bench%.*}"
            if [ -x "$executable" ]; then
                executable_name=$(basename ${executable})
                path_to_csvs="${LOGPATH}/${executable_name}"
                echo "logpath : $LOGPATH"
                echo "path_to_csvs : $path_to_csvs"
                echo "::::> Running ${executable_name} with all confs..."
                mkdir -p ${path_to_csvs}
                for conf in ./gc_configs/*.json; do
                    csv_name=$(basename ${conf} .json)
                    for items in "${ITEMS[@]}"; do
                        echo "::::::::> ${conf} with items <$items> "
                        for ((i = 1; i <= ${2}; i++)); do
                        # echo "${executable} ${conf} ${items} > ${path_to_csvs}/${executable_name}_${csv_name}_${items}_${i}.csv"
                        ${executable} ${conf} ${items} > ${path_to_csvs}/${executable_name}_${csv_name}_${items}_${i}.csv
                        done
                    done
                done
                
            else
                echo "::::> Executable ${executable} not found."
            fi
        done
        ;;
    *)
        echo "::::> Invalid argument. Please specify 'build' or 'run'."
        ;;
esac



