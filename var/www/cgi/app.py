#!/usr/bin/env python
import os
import sys
import html
import urllib.parse
import re
import fnmatch

# Enable CGI error reporting
# cgitb.enable()

def read_gitignore(root_dir):
    patterns = []
    gitignore_path = os.path.join(root_dir, '.gitignore')
    if os.path.isfile(gitignore_path):
        with open(gitignore_path, 'r') as f:
            for line in f:
                line = line.strip()
                if line and not line.startswith('#'):
                    patterns.append(line)
    return patterns

def is_ignored(path, patterns):
    for pattern in patterns:
        if re.match(fnmatch.translate(pattern), path):
            return True
    return False

def list_directory(current_path, root_dir, patterns):
    items = []
    for item in os.listdir(current_path):
        if item.startswith('.'):
            continue
        rel_path = os.path.relpath(os.path.join(current_path, item), root_dir)
        if is_ignored(rel_path, patterns):
            continue
        if os.path.isdir(os.path.join(current_path, item)):
            items.append((item + '/', rel_path + '/'))
        else:
            items.append((item, rel_path))
    return items

def render_template(template_name, context={}):
    template_path = os.path.join(os.path.dirname(__file__), 'templates', template_name)
    with open(template_path, 'r', encoding='utf-8') as f:
        template = f.read()
    # Replace placeholders in the template
    for key, value in context.items():
        if isinstance(value, str):
            template = template.replace('{' + key + '}', value)
    return template

def main():
    # Get environment variables
    method = os.environ.get('REQUEST_METHOD', 'GET')
    path_info = os.environ.get('PATH_INFO', 'cgi/')
    query_string = os.environ.get('QUERY_STRING', '')
    params = urllib.parse.parse_qs(query_string)
    print({'method': method, 'path_info': path_info, 'query_string': query_string, 'params': params})
    if path_info in ('/cgi', '/cgi/'):
        # Handle the '/cgi' route
        content = render_template('index.html')
    elif path_info in ('/cgi/post', '/cgi/post/'):
        # Handle the '/cgi/post' route
        content = render_template('post.html')
    elif path_info == '/cgi/post/json':
        data = None
        if method == 'POST':
            # Read the input data from stdin
            content_length = int(os.environ.get('CONTENT_LENGTH', '0'))
            post_data = sys.stdin.read(content_length)
            # Parse the POST data
            data = urllib.parse.parse_qs(post_data)
            # Flatten the data dictionary
            data = {key: values[0] if len(values) == 1 else values for key, values in data.items()}
            data = html.escape(str(data))
            content = render_template('json_post.html', {'data': data, 'if_data': '', '/if_data': ''})
        else:
            content = render_template('json_post.html', {'data': '', 'if_data': '<!--', '/if_data': '-->'})
    elif path_info == '/cgi/list_files':
        root_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..', '..'))
        path = params.get('path', [''])[0]
        current_path = os.path.normpath(os.path.join(root_dir, path))
        if not current_path.startswith(root_dir):
            current_path = root_dir
        patterns = read_gitignore(root_dir)
        items = list_directory(current_path, root_dir, patterns)
        items_list = ''
        for name, rel_path in items:
            item_link = f'<li><a href="/cgi/list_files?path={urllib.parse.quote(rel_path)}">{html.escape(name)}</a></li>'
            items_list += item_link
        context = {'path': html.escape(path), 'items_list': items_list}
        content = render_template('file_list.html', context)
    else:
        # Handle 404 Not Found
        print("Status: 404 Not Found")
        print("Content-Type: text/html; charset=utf-8")
        print()
        print("<h1>404 Not Found</h1>")
        return

    # Output HTTP headers and content
    print("Content-Type: text/html; charset=utf-8")
    print()
    print(content)

if __name__ == '__main__':
    main()