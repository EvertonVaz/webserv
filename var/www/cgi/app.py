#!/usr/bin/env python
import os
import sys
import html
import urllib.parse
import re
import fnmatch
import jinja2

# Enable CGI error reporting
# cgitb.enable()

# Set up Jinja2 template environment
template_loader = jinja2.FileSystemLoader(searchpath=os.path.join(os.path.dirname(__file__), 'templates'))
template_env = jinja2.Environment(loader=template_loader)

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

# Replace the render_template function
def render_template(template_name, context={}):
    template = template_env.get_template(template_name)
    return template.render(context)

def main():
    # Get environment variables
    method = os.environ.get('REQUEST_METHOD', 'GET')
    query_string = os.environ.get('QUERY_STRING', '')
    params = urllib.parse.parse_qs(query_string)
    path = params.get('path', [''])[0]  # Get 'path' parameter from query string

    if path in ('', '/'):
        # Handle the default route '/cgi?path='
        content = render_template('index.html')
    elif path in ('/post', '/post/'):
        # Handle the '/post' route '/cgi?path=/post'
        content = render_template('post.html')
    elif path == '/post/json':
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
    elif path == '/list_files':
        root_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..', '..'))
        dir_path = params.get('dir', [''])[0]  # Get 'dir' parameter for directory path
        current_path = os.path.normpath(os.path.join(root_dir, dir_path))
        if not current_path.startswith(root_dir):
            current_path = root_dir
        patterns = read_gitignore(root_dir)
        items = list_directory(current_path, root_dir, patterns)
        context = {'path': dir_path, 'items': items}
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