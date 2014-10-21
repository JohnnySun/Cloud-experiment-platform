#!/usr/bin/python2
#coding:utf-8
#Filename:check_list.py

import os, errno, shutil, datetime, time, MySQLdb

class task:
        def __init__(self):
                self.src = ''
                self.date = ''
		self.time = ''
                self.url = ''
                self.mail_addr = ''
                self.input_file = ''
                self.output_file = ''
                self.output_log = ''
                self.err_log = ''
                self.sim_flag = 1
                self.succ_flag = 0
                self.file_score = ''
                self.score = 0
		self.count = 0


# 设置主文件夹，默认是./home
home = '/usr/local/Web_Compile/home/'
sand_email = '/usr/local/Web_Compile/bin/add'
OSSim = '/usr/local/Web_Compile/OSSim/'

# 获取处于列表最上方文件的信息
def get_info(list):
	# 将字符串转换为字典
	list = eval(list)
        obj_task = task()
        obj_task.src = list[0]
        obj_task.code_type = list[1]
        obj_task.user = list[2]
        obj_task.mail_addr = list[3]
        obj_task.date = ''
        obj_task.url = ''
        # 返回obj_task对象
        return obj_task

def read():
	file_object = open('/usr/local/Web_Compile/tasklist.list', 'a+')
	buf = file_object.read()
	buf = buf.split('\n')
	file_object.close()

	# 由于列表中的最后一项是'\n'， 删除，否则会导致换行在存入时遗留
	buf.pop()

        print len(buf)
        if buf:
        # 列表不为空
                return buf
        else:
        # 列表为空
                # print "debug: %s" %(buf)
                return -1

def save(list):
	file_object = open('/usr/local/Web_Compile/tasklist.list', 'w')

	# 循环存入文件
	for i in list:
		file_object.write(i)
		file_object.write('\n')

	file_object.close()

def check_code():
	return True

def save_for_verify(mytask):
	dt = datetime.datetime.now()

	# 文件名规则：用户名_时间.code_type
        # 时间戳
        time = dt.strftime('%y-%m-%d_%H.%M.%S')
	mytask.time = time

	mytask.date = mytask.user + '_' + time + '.' + mytask.code_type
	mytask.url = home + mytask.user + '/' + mytask.code_type + '/' + time + '/'
	# 代码校验库目录


	mytask.file_score = mytask.url + mytask.user + '_' + time

        verify_url = 'verify/'

	# 相当于mkdir -p
	mkdir_p(mytask.url)
	mkdir_p(verify_url)
	# 拷贝文件到用户目录
	shutil.copy2(mytask.src, mytask.url + mytask.date)
	# link到代码校验库
	# '../'只是用来保证在测试时符号链接的连通性，正式使用时请传入绝对路径并且删除'../'
	os.symlink('../' + mytask.url + mytask.date, verify_url + mytask.date)

	#创建一个列表便于return
	# file_list = list()
	# file_list.append(date)
	# file_list.append(url)

	return mytask

# 调用gcc编译代码，以后应该根据传输进来的代码类型选择编译器
def compile(mytask):
	date = mytask.date
	url = mytask.url
	mytask.input_file = '"' + url + date + '"'

        cmd = "cp "  + mytask.input_file + " " + OSSim + "ProcSched.cpp"
        os.system(cmd)

        mytask.err_log = url + date + '.err.log'
        mytask.output_log = url + date + '.stdout.log'
        # "2>"是std_err
        cmd = 'make -C'+' ' + OSSim + ' 2>' + '"' + mytask.err_log + '"'
	# cmd = 'gcc ' + mytask.input_file + ' -o ' + mytask.output_file + ' 2>' + '"' + mytask.err_log + '"'
	# 调用gcc
	ret = os.system(cmd)
        # 获取真正的返回值，但是获取到的是Unsigned
        ret >>= 8
	print "ret is %d" %(ret)
        # 判断返回值是否是0
        if ret is 0:
                print "Compile Success,  call sand mail def"
                # 编译成功，将编译成功标识符改为1
                # mytask.succ_flag = 1
                # __debug__task__info__(mytask)
                # sandmail(mytask)
                return 0
        else:
                print "Compile failed, call sand mail def"
                __debug__task__info__(mytask)
                mytask.succ_flag = 0
                sandmail(mytask)
                return -1

def run(mytask):
        cmd = OSSim + 'OSSim' + ' ' + OSSim + 'data/processes.dat ' + OSSim + '/data/devices.dat ' + mytask.file_score + ' 2>' + mytask.err_log + ' 1>' + mytask.output_log
        ret = os.system(cmd)
	print cmd
        ret >>= 8
        if ret is 0:
                mytask.succ_flag = 1
		save_score(mytask)
		__debug__task__info__(mytask)
                sandmail(mytask)
        else:
                mytask.succ_flag = 0
		__debug__task__info__(mytask)
                sandmail(mytask)

        return 0

def save_score(mytask):
        try:
		mytask.file_score = mytask.file_score + '.score'
		file_object = open(mytask.file_score, 'a+')
                mytask.file_score = file_object.read()
                file_object.close()
                mytask.score = int(mytask.file_score)
                conn= MySQLdb.connect(host = 'localhost', user = 'root', passwd = 'whoami', db = 'test')
                cur = conn.cursor()

                # cur.execute('create database if not exists test')
                # conn.select_db('test')
		sql = "select count(*) from experiment where number  = %s"
		cur.execute(sql,mytask.user)
		result=cur.fetchall()
		print result[0][0]
		mytask.count =  result[0][0]
		print "DEBUG COUNT IS %d" %(mytask.count)
		# print "DEBUG USER IS %s" %(mytask.user)

		mytask.count = mytask.count + 1

                sql = "insert into experiment values(%s, %s, %s, %s, %s, %s, %s, %s)"
                cur.execute(sql,('NULL', mytask.user, str(mytask.count), mytask.time, str(mytask.succ_flag), mytask.input_file, mytask.output_log, str(mytask.score)))
                conn.commit()
                cur.close()
                conn.close()
        except MySQLdb.Error, e:
                print "Mysql Error %d: %s" %(e.args[0], e.args[1])

def sandmail(mytask):
        cmd = sand_email + ' "' + mytask.mail_addr + '" "' + mytask.err_log + '" ' + str(mytask.sim_flag) + ' ' + str(mytask.succ_flag) + ' "' + str(mytask.score) + '"'
        print cmd
        ret = os.system(cmd)
        ret >> 8
        if ret is 0:
                print "Sendmail Success"
		print mytask.succ_flag
                return 0

# 等同于系统mkdir -p函数
def mkdir_p(path):
	try:
		os.makedirs(path)
	except OSError as exc: # Python >2.5
		if exc.errno == errno.EEXIST and os.path.isdir(path):
			pass
		else: raise

def __debug__task__info__(mytask):
        print "Class Debug info"
        print "mail_addr: %s" %(mytask.mail_addr)
        print "url: %s" %(mytask.url)
        print "src: %s" %(mytask.src)
        print "date: %s" %(mytask.date)
        print "input_file: %s" %(mytask.input_file)
        print "output_file: %s" %(mytask.output_file)
        print "err_log: %s" %(mytask.err_log)

while True:
        time.sleep(10)
        my_list = read()
        if my_list is -1:
                print "Debug:list is empty."
        else:
                top = my_list.pop(0)

                # new_task是一个tesk对象，用于存储当前任务的数据
                new_task = get_info(top)
                new_task = save_for_verify(new_task)

                ret = compile(new_task)
		if ret is 0:
			run(new_task)
		save(my_list)

