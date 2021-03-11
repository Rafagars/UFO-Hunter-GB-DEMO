./mod2gbt audio/template.mod song 2

/opt/gbdk/bin/lcc -Wa-l -Wl-m -Wf--debug -Wl-y -Wl-w -c -o main.o main.c
/opt/gbdk/bin/lcc -Wa-l -Wl-m -Wf--debug -Wl-y -Wl-w -c -o audio/output.o audio/output.c
/opt/gbdk/bin/lcc -Wa-l -Wl-m -Wf--debug -Wl-y -Wl-w -c -o audio/gbt_player.o audio/gbt_player.s
/opt/gbdk/bin/lcc -Wa-l -Wl-m -Wf--debug -Wl-y -Wl-w -c -o audio/gbt_player_bank1.o audio/gbt_player_bank1.s 

/opt/gbdk/bin/lcc -Wa-l -Wl-m -Wf--debug -Wl-y -Wl-w -Wl-yt1 -Wl-yo4 -Wl-ya0 -o ufo_hunter.gb main.o audio/output.o audio/gbt_player.o audio/gbt_player_bank1.o

rm *.o *.lst