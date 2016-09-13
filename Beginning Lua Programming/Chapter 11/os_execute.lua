print("os.execute returned:", os.execute("ls -sh")) -- "ls" is a
  -- directory listing program available on Unix-like systems.
print("HOME environment variable:", os.getenv("HOME"))
assert(os.setlocale("pt_BR", "numeric"))
print(math.pi) -- Prints with a comma!
os.exit()
print("This will never get printed!")
