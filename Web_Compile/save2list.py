#!/usr/bin/python2
#coding:utf-8
#Filename: save2list.py

import sys

def getname(code_path, type, user, mail):
	app_dict = dict()
	app_dict[0] = code_path
	app_dict[1] = type
	app_dict[2] = user
        app_dict[3] = mail
	return app_dict

def save_task(task):
	file_object = open("/usr/local/Web_Compile/tasklist.list", 'a')
	data = str(task) + '\n'
	file_object.write(data)
	file_object.close()

output = getname(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4])
save_task(output)
