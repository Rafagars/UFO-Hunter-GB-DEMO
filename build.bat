cd audio

mod2gbt video_demo.mod song -c 2

cd  ..

C:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o main.o main.c
C:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o audio/output.o audio/output.c
C:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o audio/gbt_player.o audio/gbt_player.s
C:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o audio/gbt_player_bank1.o audio/gbt_player_bank1.s

C:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -Wl-yt1 -Wl-yo4 -Wl-ya0 -o ufo_hunter.gb main.o audio/output.o audio/gbt_player.o audio/gbt_player_bank1.o

del *.o *.lst

pause