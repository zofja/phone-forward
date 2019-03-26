#!/bin/bash

prog=$1
redirs=$2
num=$3
red="\e[31m"
NC="\e[0m"

cp $redirs tmp

regex="^[0-9>[:space:]]*$"

if [ ! -f $prog ]
then
    >&2 echo -e "${red}Plik $prog nie istnieje.${NC}"
    exit 1
elif [ ! -f $redirs ]
then
    >&2 echo -e "${red}Plik $redirs nie istnieje.${NC}"
    exit 1
elif [[ ! $num =~ ^[0-9]+$ ]];
then
    >&2 echo -e "${red}$num nie jest numerem.${NC}"
    exit 1
elif [[ ! $(<$redirs) =~ $regex ]];
then
    >&2 echo -e "${red}Plik $redirs zawiera niepoprawny ciąg znaków.${NC}"
    exit 1
else

    sed -i '1s/^/NEW A\n/' tmp
    echo ? $num >> tmp
    $prog < tmp 1>tmp1 2>err

    if [[ $(tail --lines=1 err) != "" ]]; then
        echo -e "${red}Plik $redirs zawiera niepoprawny ciąg znaków.${NC}"
        exit 1
    fi

    sed -i '$d' tmp
    while read p; do
        echo $p ? >> tmp
    done < tmp1

    $prog < tmp > tmp2
    ctr=1
    while read p; do
        if [ "$num" = "$p" ]; then
            sed "${ctr}q;d" tmp1
        fi
        ((ctr++))
    done < tmp2

    ((l++))
    rm tmp tmp1 tmp2
fi
