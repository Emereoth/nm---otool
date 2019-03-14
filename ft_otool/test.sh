./ft_otool ../testfiles/32bittest > mylog
otool -t ../testfiles/32bittest > truelog
diff mylog truelog
rm truelog mylog

./ft_otool ../testfiles/dylibtest.dylib > mylog
otool -t ../testfiles/dylibtest.dylib > truelog
diff mylog truelog
rm truelog mylog

./ft_otool ../testfiles/libkmod.a > mylog
otool -t ../testfiles/libkmod.a > truelog
diff mylog truelog
rm truelog mylog

./ft_otool ../testfiles/libtest.a > mylog
otool -t ../testfiles/libtest.a > truelog
diff mylog truelog
rm truelog mylog

./ft_otool ../testfiles/liby.a > mylog
otool -t ../testfiles/liby.a > truelog
diff mylog truelog
rm truelog mylog

./ft_otool ../testfiles/lstest > mylog
otool -t ../testfiles/lstest > truelog
diff mylog truelog
rm truelog mylog

./ft_otool ../testfiles/pythontest > mylog
otool -t ../testfiles/pythontest > truelog
diff mylog truelog
rm truelog mylog

./ft_otool ../testfiles/sotest.so > mylog
otool -t ../testfiles/sotest.so > truelog
diff mylog truelog
rm truelog mylog

./ft_otool ../testfiles/corr_missing_parts
./ft_otool ../testfiles/corr_no_magicnb
./ft_otool ../testfiles/corr_stringtab