#!/usr/bin/env python
# coding: cp932
# 
# usage: python cccc2csv.py
""" cccc.exeコマンドを使って、メトリクス集計されたデータを、より見やすいCSVファイル化する。
    CSVファイル化すれば、Excelで簡単に見れる理屈。
    
    ccccについては、以下を参照。
    - SourceForge.net: C and C++ Code Counter
        http://sourceforge.net/projects/cccc
    - UNIXの部屋 検索:cccc (*BSD/Linux/Solaris)
        http://x68000.q-e-d.net/~68user/unix/pickup?cccc
"""
import sys
try:
    import elementtree.ElementTree as ET    # Python 2.4
except ImportError:
    import xml.etree.ElementTree as ET      # Python 2.5
import fnmatch
import os
import os.path
import time, datetime

# CSV書き込みするファイルオブジェクト(Noneだと、標準出力)
g_f = None
# CSV書き込みする際のヘッダタイトル
g_o_def = ['module', 'name', 'LOC', 'COM', 'MVG', 'declaration', 'definition']

def usage():
    print """
＜＜＜%s の使用方法＞＞＞
cccc.exeを呼び出した後、'.cccc'フォルダが出来る。
メトリクス集計された'*.xml'を読み出し、CSVファイル化する。

--l ['.cccc'フォルダの場所を指定する('.cccc'がデフォルト)]
--s [結果を保存するCSVファイル名] : 指定しないと標準出力する
--help or /? : 使い方の説明
""" % (sys.argv[0])

def prt(msg):
    """g_fに対して、文字列を書き込む"""
    if g_f == None:     print msg
    else:               print >>g_f, msg

def prt_head(keys):
    """g_fに対して、ヘッダ行相当を書き込む"""
    global g_f
    s = ""
    for k in keys:
        s = s + "," + ('"%s"' % k)
    if g_f == None:     print s[1:]
    else:               print >>g_f, s[1:]

def prt_line(o, keys):
    """g_fに対して、中身相当を書き込む"""
    global g_f
    s = ""
    for k in keys:
        s = s + "," + ('"%s"' % o.get(k, ''))
    if g_f == None:     print s[1:]
    else:               print >>g_f, s[1:]

def project_summary(fpath):
    """プロジェクトの概要を書き出す
        -fpath  cccc.xmlを指定しましょう
    """
    o = ET.parse(fpath)
    
    s = o.findtext("timestamp").strip()
    t1 = time.strptime(s)
    t2 = datetime.datetime(*t1[:6])
    prt('メトリクス計測された時間,%s' % t2.isoformat(' '))

    NOM = o.find("project_summary/number_of_modules")
    prt("モジュール数," + NOM.attrib['value'] + ",モジュール")
    LOC = o.find("project_summary/lines_of_code")
    prt("コード行数," + LOC.attrib['value'] + ",行")
    COM = o.find("project_summary/lines_of_comment")
    prt("コメント行数," + COM.attrib['value'] + ",行")
    REJ = o.find("project_summary/rejected_lines_of_code")
    prt("解析できなかった行数," + REJ.attrib['value'] + ",行")

def xml_parse(fpath):
    """指定モジュール(fname)の各メソッドのメトリクスを書き出す
        -fpath  *.xmlを指定しましょう(cccc.xmlは不要)
    """
    tree = ET.parse(fpath)
    
    mod_name = os.path.splitext(os.path.basename(fpath))[0]
    
    funcS = tree.findall("*/member_function")
    for func in funcS:
        o = {}
        o['module'] = mod_name
        for e in func:
            #print e.tag
            if e.tag == 'name':
                o['name'] = e.text
            elif e.tag == 'lines_of_code':
                o['LOC'] = e.attrib['value']
            elif e.tag == 'McCabes_cyclomatic_complexity':
                o['MVG'] = e.attrib['value']
            elif e.tag == 'lines_of_comment':
                o['COM'] = e.attrib['value']
            elif e.tag == 'extent':
                s = e.find('description').text
                e2 = e.find('source_reference')
                o[s] = '%s:%s' % (e2.attrib['file'], e2.attrib['line'])
        #print o
        
        prt_line(o, g_o_def)

def main():
    load_dir = ".cccc"
    save_path = "metrics.csv"
    
    if len(sys.argv) > 1:
        # 引数解析
        i = 1
        while i < len(sys.argv):
            s = sys.argv[i]
            print i, "=", s
            i = i + 1
            if s == "--l":      load_dir = sys.argv[i]
            elif s == "--s":    save_path = sys.argv[i]
            elif (s == "--help") or (s == "/?"):
                                usage()
                                return
            else:               continue
            i = i + 1
    
    if len(save_path) != 0:
        global g_f
        g_f = file(save_path, 'w')
    
    try:
        # プロジェクトの概要(cccc.xml)を書き出す
        project_summary(load_dir + "/cccc.xml")
        prt("")

        # 全てのモジュールの詳細(*.xml)を書き出す
        prt_head(g_o_def)
        for root, dirs, files in os.walk(load_dir):
            for fname in files:
                if fnmatch.fnmatch(fname, "*.xml"):
                    if fname != "cccc.xml":
                        fpath = os.path.join(root, fname)
                        print fpath
                        xml_parse(fpath)
    finally:
        if g_f != None:
            g_f.close()

if __name__ == "__main__":
    ret = main()