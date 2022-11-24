#!/bin/bash

# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

# update to fluffos driver docs directory path
driverDocs=driver/docs
# update to mudlib docs directory path
libDocs=lib/doc

# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!



types=("apply" "efun" "lpc")    # exclude concepts, driver, stdlib, and zh-CN

for type in ${types[@]} ; do
    mkdir -p ${libDocs}/${type} # ensure mudlib dir exists

    for dir in ${driverDocs}/${type}/*/ ; do
        dir=${dir%*/}           # remove trailing /

        for file in ${dir}/*.md ; do
            filename=${file##*/}    # grab everything after last /

            [[ ${filename} =~ ^__ ]] && continue    # ignore obsolete apply __INIT
            [[ ${filename} =~ ^README ]] && continue    # ignore README

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