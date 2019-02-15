./ft_nm ../testfiles/32bittest > mylog
nm ../testfiles/32bittest > truelog
diff mylog truelog
rm truelog mylog

./ft_nm ../testfiles/dylibtest.dylib > mylog
nm ../testfiles/dylibtest.dylib > truelog
diff mylog truelog
rm truelog mylog

./ft_nm ../testfiles/libkmod.a > mylog
nm ../testfiles/libkmod.a > truelog
diff mylog truelog
rm truelog mylog

./ft_nm ../testfiles/libtest.a > mylog
nm ../testfiles/libtest.a > truelog
diff mylog truelog
rm truelog mylog

./ft_nm ../testfiles/liby.a > mylog
nm ../testfiles/liby.a > truelog
diff mylog truelog
rm truelog mylog

./ft_nm ../testfiles/lstest > mylog
nm ../testfiles/lstest > truelog
diff mylog truelog
rm truelog mylog

./ft_nm ../testfiles/pythontest > mylog
nm ../testfiles/pythontest > truelog
diff mylog truelog
rm truelog mylog

./ft_nm ../testfiles/sotest.so > mylog
nm ../testfiles/sotest.so > truelog
diff mylog truelog
rm truelog mylog

./ft_nm ../testfiles/corr_missing_parts
./ft_nm ../testfiles/corr_no_magicnb
./ft_nm ../testfiles/corr_stringtab
./ft_nm ../testfiles/audiotest