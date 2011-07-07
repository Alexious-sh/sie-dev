if exist %2\ff.bin %1\elf2vkp.exe %2\ELFLoader.elf %2\temp.vkp %2\ff.bin
if not exist %2\ff.bin %1\elf2vkp.exe %2\ELFLoader.elf %2\temp.vkp
if exist %2\swilib.vkp copy /b %2\swilib.vkp+%2\temp.vkp %2\ElfPack.vkp
if not exist %2\swilib.vkp rename %2\temp.vkp ElfPack.vkp
del %2\temp.vkp
