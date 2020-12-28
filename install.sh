if [[ -z "$1" ]]
then
    echo "arg is root for include dir"
    exit 1
fi

( \
    echo "uninstall" \
    && \
    rm -rv $1/include/cxx_util \
    && \
    echo "install" \
    && \
    cp -rv include $1 \
    &&
    cp -v compile_flags.txt $1/include/cxx_util \
)