#! /usr/bin/fish
echo "please input version:"
read ver
zip -r  record_module_$ver.zip src/ README.md  doc/ CHANGELOG 