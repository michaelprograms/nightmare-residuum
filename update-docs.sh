#!/usr/bin/env bash

# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

# update to fluffos driver docs directory path
driverDocs=driver/docs
# update to mudlib docs directory path
libDocs=lib/doc

# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!



types=("apply" "efun")    # exclude concepts, driver, stdlib, and zh-CN
for type in ${types[@]} ; do
    mkdir -p ${libDocs}/${type} # ensure mudlib dir exists

    for dir in ${driverDocs}/${type}/*/ ; do
        dir=${dir%*/}           # remove trailing /

        for file in ${dir}/*.md ; do
            filename=${file##*/}    # grab everything after last /

            [[ ${filename} =~ ^__ ]] && continue        # ignore obsolete apply __INIT
            [[ ${filename} =~ ^README ]] && continue    # ignore README
            [[ ${filename} =~ ^index ]] && continue     # ignore index

            # new doc file path
            newDoc=${libDocs}/${type}/${filename%.md}
            tmpDoc=${newDoc}.tmp

            cp ${file} ${tmpDoc}        # copy to temporary file
            sed -i '1,4 d' ${tmpDoc}    # trim beginning of file
            # update doc if new file is different
            cmp --silent ${tmpDoc} ${newDoc} || echo Updating ${newDoc} && cp ${tmpDoc} ${newDoc}
            rm ${tmpDoc}                # remove temporary file

        done
    done
done

types=("lpc/constructs" "lpc/preprocessor" "lpc/types")
for type in ${types[@]} ; do
    mkdir -p ${libDocs}/${type} # ensure mudlib dir exists

    echo type ${type}
    for dir in ${driverDocs}/${type}/ ; do
        dir=${dir%*/}           # remove trailing /
        echo dir ${dir}

        for file in ${dir}/*.md ; do
            filename=${file##*/}    # grab everything after last /
            echo filename ${filename}
            [[ ${filename} =~ ^__ ]] && continue        # ignore obsolete apply __INIT
            [[ ${filename} =~ ^README ]] && continue    # ignore README
            [[ ${filename} =~ ^index ]] && continue     # ignore index

            # new doc file path
            newDoc=${libDocs}/${type}/${filename%.md}
            tmpDoc=${newDoc}.tmp

            cp ${file} ${tmpDoc}        # copy to temporary file
            sed -i '1,4 d' ${tmpDoc}    # trim beginning of file
            # update doc if new file is different
            cmp --silent ${tmpDoc} ${newDoc} || echo Updating ${newDoc} && cp ${tmpDoc} ${newDoc}
            rm ${tmpDoc}                # remove temporary file

        done
    done
done