TmStart = os.time{year = 2001, month = 1, day = 1, hour = 0}
TmNow = os.time()
io.write(os.difftime(TmNow, TmStart), " seconds have passed since ",
  os.date("%d %B %Y", TmStart), "\n")
