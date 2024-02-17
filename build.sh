path='Debug'
if [ "$#" -ge 1 ]; then
  path='Release'
fi
if [ ! -d "xbuild/$path" ]; then
  mkdir -p "xbuild/$path" 
fi
cd "xbuild/$path"
cmake ../.. -DCMAKE_BUILD_TYPE=$path
cmake --build .
cp compile_commands.json ../..

