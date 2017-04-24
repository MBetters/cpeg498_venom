import web
from web import form
import json
import os
import shutil

urls = (
    '/', 'index',
    '/js/(.*)', 'js',
	'/update_action', 'update_action',
	'/info', 'info',
	'/images/(.*)', 'images',
	'/update_img', 'update_img'
)

app = web.application(urls, globals())

class index:
    #This server class serves the static index HTML page to the client
    def GET(self):
        index_page = open('./static/index.html')
        return index_page.read()
        
class js:
    #This server class serves javascript files to the client
    def GET(self, js_filename):
        js_file = open('./js/' + js_filename)
        return js_file.read()
        
class update_action:
    def POST(self):
        data = web.input()
        action = data['action']
        print "action: " + action
        latest_info = {"action": action}
        with open('info.json', 'w') as info_json_file:
        	json.dump(latest_info, info_json_file)
        #redirect to the index page
        #Note: I could've put '/' as the argument to web.seeother,
        #      but I wanted the redirect to know to send the client to
        #      the index page using the https protocol explicitly,
        #      since Status 303 redirects default to using http
        raise web.seeother('https://cpeg498-spider-mbetters.c9users.io/')
        
class images:
    def GET(self, img_name):
        img_ext = img_name.split(".")[-1]

        cType = {
            "png":"images/png",
            "jpg":"images/jpeg",
            "gif":"images/gif",
            "ico":"images/x-icon"}

        if img_name in os.listdir('images'):
            web.header("Content-Type", cType[img_ext])
            return open('images/%s' % img_name, "rb").read()
        else:
            raise web.notfound()
        
class update_img:
    def POST(self):
        print "update_img"
        img_data = web.input()['img']
        #print img_data
        #print "$$$$$$$$$$$$$$$$$$$$$$$"
        with open('images/latest.jpg', 'wb') as img:
            #This isn't working.
            #Try http://stackoverflow.com/questions/6728077/convert-python-str-unicode-object-to-binary-hex-blob
            #instead???
            img.write(img_data)
        #TODO: Change the next line to a POST-success status code, to be faster.
        raise web.seeother('https://cpeg498-spider-mbetters.c9users.io/')

def get_dict_with_stringified_keys(dictionary):
    dict_to_return = {}
    for k in dictionary:
        dict_to_return[str(k)] = dictionary[k]
    return dict_to_return
		
class info:
    def GET(self):
        latest_info = {}
        with open('info.json') as info_json_file:
            latest_info = json.load(info_json_file)
        web.header('Content-Type', 'application/json')
        return json.dumps(latest_info)

if __name__=="__main__":
    #web.internalerror = web.debugerror
    app.run()