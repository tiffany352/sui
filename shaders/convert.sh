#!/usr/bin/env bash

echo "// Automatically generated file, do not modify";

for f in shaders/*.vert shaders/*.frag; do
    echo "";
    name=$(printf $(basename $f) | tr '.' '_');
    printf "const char sui_shader_"$name"[] = \"";
    while read line; do
        printf "%s\\\\n" "$line";
    done < $f;
    echo "\";";
done
