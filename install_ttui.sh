if [ -d "ttui" ] ; then
    cd "ttui"
    git fetch origin
    log=$(git log HEAD..origin/main --oneline)
    cd ..
fi

if [ "${log}" != "" -o ! -d "ttui" ] ; then
    echo "Removing \"ttui/\" and \"include/ttui/\" directories..."
    rm -rf "ttui"
    rm -rf "include/ttui"

    git clone https://github.com/LioQing/ttui
fi

cd "ttui"

if [ ! -d "build/release/lib/libttui.a" ] ; then
    make release
fi

if [ ! -d "build/debug/lib/libttui.a" ] ; then
    make debug
fi

cd ..

echo "Finished building ttui." 