import sys
import os

sys.path.append('/opt/spider_server/venv/lib/python2.7/site-packages')

import web
from web import form
import json
import shutil

project_path = "/opt/spider_server"

urls = (
    '/', 'index',
    '/js/(.*)', 'js',
	'/update_action', 'update_action',
	'/info', 'info',
	'/images/(.*)', 'images',
	'/update_img', 'update_img'
)

application = web.application(urls, globals()).wsgifunc()

class index:
    #This server class serves the static index HTML page to the client
    def GET(self):
        index_page = open(project_path + '/static/index.html')
        return index_page.read()
        
class js:
    #This server class serves javascript files to the client
    def GET(self, js_filename):
        js_file = open(project_path + '/js/' + js_filename)
        return js_file.read()
        
class update_action:
    def POST(self):
        data = web.input()
        action = data['action']
        print "action: " + action
        latest_info = {"action": action}
        with open(project_path + '/info.json', 'w') as info_json_file:
        	json.dump(latest_info, info_json_file)
        #redirect to the index page
        #Note: I could've put '/' as the argument to web.seeother,
        #      but I wanted the redirect to know to send the client to
        #      the index page using the https protocol explicitly,
        #      since Status 303 redirects default to using http
        raise web.seeother('/')
        
class images:
    def GET(self, img_name):
        img_ext = img_name.split(".")[-1]

        mime_types = {
            "png":"images/png",
            "jpg":"images/jpeg",
            "gif":"images/gif",
            "ico":"images/x-icon"}

        if img_name in os.listdir(project_path + '/images'):
            web.header("Content-Type", mime_types[img_ext])
            return open(project_path + '/images/%s' % img_name, "rb").read()
        else:
            raise web.notfound()
        
class update_img:
    def POST(self):
        print "update_img"
        img_data = web.input()['img']
        #print img_data
        #print "$$$$$$$$$$$$$$$$$$$$$$$"
        with open(project_path + '/images/latest.jpg', 'wb') as img:
            #This isn't working.
            #Try http://stackoverflow.com/questions/6728077/convert-python-str-unicode-object-to-binary-hex-blob
            #instead???
            img.write(img_data)
        #TODO: Change the next line to a POST-success status code, to be faster.
        raise web.seeother('/')

def get_dict_with_stringified_keys(dictionary):
    dict_to_return = {}
    for k in dictionary:
        dict_to_return[str(k)] = dictionary[k]
    return dict_to_return
		
class info:
    def GET(self):
        latest_info = {}
        with open(project_path + '/info.json') as info_json_file:
            latest_info = json.load(info_json_file)
        web.header('Content-Type', 'application/json')
        return json.dumps(latest_info)

#Commenting this block out for the apache2/mod_wsgi server
#if __name__=="__main__":
#    web.internalerror = web.debugerror
#    app.run()
