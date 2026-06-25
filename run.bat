//(AI)
@echo off
chcp 65001 >nul
//编译
gcc main.c common.c user.c post.c join.c -o main.exe -finput-charset=utf-8 -fexec-charset=utf-8

if %errorlevel% equ 0 (
    echo 编译成功！启动程序...
    echo ---------------------------
    main.exe
) else (
    echo 编译失败！请检查代码
    pause
)