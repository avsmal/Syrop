#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os

def backupSettings ():
	pass

def setupSettings (settings):
	cleanupSettings()
	# получаем имя файла настройки ~/.bashrc
	name = os.path.expanduser("~/.bashrc")
	bashrc = open(name, "a")
	# пишем все в конец файла и расставляем метки для удаления
	bashrc.write("\n#syrop begin\n")

	if "http" in settings:
		bashrc.write("http_proxy=" + settings["http"] + "\n")
		bashrc.write("export http_proxy\n")
	
	if "https" in settings:
		bashrc.write("https_proxy=" + settings["https"] + "\n")
		bashrc.write("export https_proxy\n")
		
	if "ftp" in settings:
		bashrc.write("ftp_proxy=" + settings["ftp"] + "\n")
		bashrc.write("export ftp_proxy\n")
	# ставим конечную метку
	bashrc.write("\n#syrop end\n")
	bashrc.close()

def cleanupSettings ():
	name = os.path.expanduser("~/.bashrc")
	# переименовываем bashrc
	os.rename(name, name + ".old")
	# открываем для чтения
	bashrcold = open(name + ".old", "r")
	# и создаем новый bashrc
	bashrcnew = open(name, "w")
	toDel = False
	# читаем построчно исходный bashrc
	for line in bashrcold.readlines():
		# если прочитанная строка совпадает с начальной меткой
		# значит начиная с этой строки и до конца файла или конечной метки
		# все строки должны быть удалены
		if line.strip(None) == "#syrop begin":
			toDel = True
		# если не в секции для удаления, то просто переписываем
		if not toDel:
			bashrcnew.write(line)
		# если прочитали конечную метку, то начиная с нее строки не удаляются
		if line.strip(None) == "#syrop end":
			toDel = False
	bashrcnew.close()
	bashrcold.close()
	# удаляем старый bashrc
	os.remove(name + ".old")
