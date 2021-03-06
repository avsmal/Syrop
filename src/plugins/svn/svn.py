#!/usr/bin/env python
# -*- coding: utf-8 -*-
import ConfigParser, os, re

def backupSettings ():
	pass

def setupSettings (settings):

    user_pass = {}
    if "user_pass" in settings:
        res = settings["user_pass"].split(":")
        user_pass["user"] = res[0]
        user_pass["password"] = res[1]

	#проверяем что есть нужные настройки
	if 'http' in settings:
		#сдвигаем global секцию в конец файла
		moveGlobalToEnd()
		#достаем имя конфигурационного файла
		name = os.path.expanduser('~/.subversion/servers')
		#делаем мап с найтройками
		params = splitProxyString(settings['http'])
		#читаем файл настроек (он в ини формате, так что используем ConfigParser)
		config = ConfigParser.RawConfigParser()
		config.read([name])
		#открываем сам файл
		cfg = open(name, 'a')
		#ставим метку начала секции
		cfg.write('#syrop begin\n')
		#все настройки пишем в глобальную секцию, группы пока не поддерживаются
		if not config.has_section('global'):
			cfg.write('[global]\n')
		
		#проверяем наличие каждой переменной в global секции, если ее там нет
		#устанавливаем, а если есть, то выводим предупреждение
		if config.has_option('global', 'http-proxy-host'):
			print 'WARN: http-proxy-host is set already'
		elif 'host' in params:
			cfg.write('http-proxy-host=' + params['host'] + '\n')
		
		if config.has_option('global', 'http-proxy-port'):
			print 'WARN: http-proxy-port is set already'
		elif 'port' in params:
			cfg.write('http-proxy-port=' + params['port'] + '\n')
		
		if config.has_option('global', 'http-proxy-username'):
			print 'WARN: http-proxy-username is set already'
		elif 'user' in user_pass:
			cfg.write('http-proxy-username=' + user_pass['user'] + '\n')
		
		if config.has_option('global', 'http-proxy-password'):
			print 'WARN: http-proxy-password is set already'
		elif 'password' in user_pass:
			cfg.write('http-proxy-password=' + user_pass['password'] + '\n')
		
		#выставляем метку конца секции и закрываем файл
		cfg.write('#syrop end\n')
		cfg.close()
	else:
		print 'http parameters expected for SVN plugin'

def cleanupSettings ():
	name = os.path.expanduser('~/.subversion/servers')
	os.rename(name, name + '.old')
	oldconf = open(name + '.old', 'r')
	newconf = open(name, 'w')
	toDel = False
	for line in oldconf.readlines():
		if line.strip(None) == '#syrop begin':
			toDel = True
		if not toDel:
			newconf.write(line)
		if line.strip(None) == '#syrop end':
			toDel = False
	newconf.close()
	oldconf.close()
	os.remove(name + '.old')
	
def sectionString (string):
    m = re.match(r"^\[\s*(.+)\s*\]$", string.strip(None))
    if m is not None:
        return m.group(1)
        return None

#сдвигаем global секцию конфигурационного файла в конец,
#чтобы можно было добавлять в нее простым дописываем в файл
def moveGlobalToEnd ():
	name = os.path.expanduser('~/.subversion/servers')
	os.rename(name, name + '.old')
	oldconf = open(name + '.old', 'r')
	newconf = open(name, 'w')
	globalSection = []
	inGlobal = False
	for line in oldconf.readlines():
		section = sectionString(line)
		
		if (section is not None) and (section == 'global'):
			inGlobal = True
		elif section is not None:
			inGlobal = False
			
		if inGlobal:
			globalSection.append(line)
		else:
			newconf.write(line)
			
	for line in globalSection:
		newconf.write(line)
		
	newconf.close()
	oldconf.close()
	os.remove(name + '.old')
	

#функция ожидает строку вида [user:password@]server[:port]
def splitProxyString (conf):
	params = {}
	m = re.match(r"^([^:@]+){1}[:]([^:@]+){1}$", conf.strip(None))

	if m is not None:
		if m.group(1) is not None:
			params["host"] = m.group(1)
		if m.group(2) is not None:
			params["port"] = m.group(2)

	return params
