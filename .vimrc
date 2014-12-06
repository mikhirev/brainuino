setlocal shiftwidth=2
setlocal softtabstop=2
setlocal expandtab
autocmd BufNewFile,BufRead *.ino setlocal ft=cpp
autocmd BufWritePost *.ino,*.cpp,*.h silent !ctags --language-force=C++ *.ino *.cpp *.h
