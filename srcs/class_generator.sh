#!/bin/bash

NAME="$1.hpp";

if test -f $NAME; then
        echo "$NAME already exist";
else
        vi +Stdheader $NAME +12d +wq;
        echo -n "#ifndef " >> $NAME;
        python -c "print('$1_h'.upper())" >> $NAME;
        echo -n "# define " >> $NAME;
        python -c "print('$1_h\n'.upper())" >> $NAME;
        printf "# include <iostream>\n\n" >> $NAME
        echo "class $1" >> $NAME;
        printf "{\n\tpublic:\n" >> $NAME;
        printf "\t\t$1(void);\n" >> $NAME;
        printf "\t\t$1($1 const & src);\n" >> $NAME;
        printf "\t\t$1 &operator=(const $1 &rhs);\n" >> $NAME;
        printf "\t\t~$1(void);\n" >> $NAME;
        printf "\n\tprivate:\n\t\t\n" >> $NAME;
        printf "};\n" >> $NAME;
        printf "\n\n#endif" >> $NAME;
fi

NAME="$1.cpp";

if test -f $NAME; then
        echo "$NAME already exist";
else
        vi +Stdheader $NAME +12d +wq;
        printf "#include \"$1.hpp\"\n\n" >> $NAME;
        printf "$1::$1(void)\n{\n}\n\n" >> $NAME;
        printf "$1::$1($1 const & src)\n" >> $NAME;
        printf "{\n\t*this = src;\n}\n\n" >> $NAME;
        printf "$1 &$1::operator=($1 const &rhs)\n{\n\treturn *this;\n}\n\n" >> $NAME;
        printf "$1::~$1(void)\n{\n}\n\n" >> $NAME;

fi
