#!/usr/bin/env python
import mimetypes
import os
import re
import sys
import uuid
import jinja2
import fnmatch
import urllib.parse
import base64

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

def get_image_base64(image_path):
    with open(image_path, "rb") as image_file:
        encoded_string = base64.b64encode(image_file.read()).decode("utf-8")
    return encoded_string


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

def list_files(params):
    root_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..', '..'))
    dir_path = params.get('dir', [''])[0]  # Get 'dir' parameter for directory path
    current_path = os.path.normpath(os.path.join(root_dir, dir_path))
    if not current_path.startswith(root_dir):
        current_path = root_dir
    patterns = read_gitignore(root_dir)

    if os.path.isfile(current_path):
        mime_type, _ = mimetypes.guess_type(current_path)
        if mime_type and mime_type.startswith('image'):
            image_base64 = get_image_base64(current_path)
            context = {
                'file_path': dir_path,
                'image_base64': image_base64,
                'mime_type': mime_type,
                'parent_dir': os.path.dirname(dir_path)
            }
            content = render_template('binary_file.html', context)
        else:
            file_name = os.path.basename(current_path)
            parent_dir = os.path.dirname(dir_path)
            context = {'file_path': dir_path, 'file_name': file_name, 'parent_dir': parent_dir, 'mime_type': mime_type}
            print(context, file=sys.stderr)
            content = render_template('binary_file.html', context)
    elif os.path.isdir(current_path):
        items = list_directory(current_path, root_dir, patterns)
        context = {'path': dir_path, 'items': items}
        content = render_template('file_list.html', context)

    return content



# Replace the render_template function
def render_template(template_name, context={}):
    template = template_env.get_template(template_name)
    return template.render(context)

def handle_json_post(method):
    data = {}
    if method == 'POST':
        # Read POST data
        content_length = int(os.environ.get('CONTENT_LENGTH', '0'))
        post_data = sys.stdin.read(content_length)
        # Parse POST data
        data = urllib.parse.parse_qs(post_data)
        # Flatten the dictionary
        data = {key: values[0] if len(values) == 1 else values for key, values in data.items()}
        content = render_template('json_post.html', {'data': data})
    else:
        content = render_template('json_post.html', {'data': '', 'if_data': '<!--', '/if_data': '-->'})
    return content

def handle_multipart_post(method):
    data = {}
    if method == 'POST':
        content_type = os.environ.get('CONTENT_TYPE', '')
        content_length = int(os.environ.get('CONTENT_LENGTH', '0'))
        post_data = sys.stdin.buffer.read(content_length)  # Ler como bytes

        # Extrai o boundary do header Content-Type
        boundary_matches = re.findall(br'boundary=(.*)', content_type.encode())
        if not boundary_matches:
            data['error'] = 'Boundary não encontrado.'
            return render_template('multipart_post.html', {'data': data})
        boundary = boundary_matches[0].strip()

        # Separa as partes com base no boundary
        delimiter = b'--' + boundary
        parts = post_data.split(delimiter)
        files = {}

        for part in parts:
            part = part.strip(b'\r\n')
            if not part or part == b'--':
                continue
            headers, _, body = part.partition(b'\r\n\r\n')
            header_lines = headers.decode().split('\r\n')
            disposition = None
            field_name = None
            filename = None

            for header in header_lines:
                if header.lower().startswith('content-disposition'):
                    disposition_match = re.findall(r'form-data; name="([^"]+)"(?:; filename="([^"]+)")?', header)
                    if disposition_match:
                        field_name, filename = disposition_match[0]

            if filename:
                # Gera um nome único para o arquivo
                unique_filename = f"{uuid.uuid4().hex}_{filename}"
                upload_dir = os.path.join(os.path.dirname(__file__), 'uploads')
                os.makedirs(upload_dir, exist_ok=True)
                file_path = os.path.join(upload_dir, unique_filename)

                # Salva o arquivo
                with open(file_path, 'wb') as f:
                    f.write(body)

                files[field_name] = unique_filename
            else:
                # Campos de formulário normais
                try:
                    data[field_name] = body.decode().strip()
                except UnicodeDecodeError:
                    data[field_name] = body  # Manter como bytes se não for decodificável

        data['files'] = files

    content = render_template('multipart_post.html', {'data': data})
    return content

def handle_xml_post(method):
    data = {}
    if method == 'POST':
        content_length = int(os.environ.get('CONTENT_LENGTH', '0'))
        post_data = sys.stdin.read(content_length)
        # Parse XML data
        data = {'xmlData': post_data}
    content = render_template('xml_post.html', {'data': data})
    return content



def main():
    # Get environment variables
    method = os.environ.get('REQUEST_METHOD', 'GET')
    path = os.environ.get('PATH_INFO', '')
    query_string = os.environ.get('QUERY_STRING', '')
    params = urllib.parse.parse_qs(query_string)

    lista = list(filter(None, path.split("/")))
    # print("\n", lista, "\n", file=sys.stderr)



    path_len = len(lista)
    if (path_len > 1):
        path = path[path.find("/", path_len):]
    # print("\n", path, "\n", file=sys.stderr)
    if path_len == 1 and query_string == '':
        content = render_template('index.html')
    elif path in ('/post', '/post/'):
        content = render_template('post.html')
    elif path == '/post/json':
        content = handle_json_post(method)
    elif path == '/post/multipart':
        content = handle_multipart_post(method)
    elif path == '/post/xml':
        content = handle_xml_post(method)
    elif query_string != '' or path == '/list_files':
        content = list_files(params)
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