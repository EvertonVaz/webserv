#!/usr/bin/env python3
import os
import sys
import re
import fnmatch
from urllib.parse import parse_qs, quote
import html

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

def main():
    try:
        root_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..', '..'))

        query_string = os.environ.get('QUERY_STRING', '')
        query_params = parse_qs(query_string)
        path = query_params.get('path', [''])[0]
        current_path = os.path.normpath(os.path.join(root_dir, path))

        if not current_path.startswith(root_dir):
            current_path = root_dir

        patterns = read_gitignore(root_dir)

        if os.path.isfile(current_path):
            with open(current_path, 'r') as f:
                content = f.read()
            print("Content-Type: text/html")
            print()
            print("<!DOCTYPE html>")
            print("<html lang='pt-BR'>")
            print("<head>")
            print("    <meta charset='UTF-8'>")
            print("    <meta name='viewport' content='width=device-width, initial-scale=1.0'>")
            print("    <title>Teste CGI Python</title>")
            print("    <link rel='stylesheet' href='./cgi/styles.css'>")
            print("</head>")
            print("<body>")
            print("    <div class='container'>")
            print(f"        <h1>Conteúdo de {html.escape(path)}</h1>")
            print("        <pre>")
            print(html.escape(content))
            print("        </pre>")
            print(f"        <a href='?path={quote(os.path.dirname(path))}' class='button'>Voltar</a>")
            print("    </div>")
            print("</body>")
            print("</html>")
        else:
            items = list_directory(current_path, root_dir, patterns)
            print("Content-Type: text/html")
            print()
            print("<!DOCTYPE html>")
            print("<html lang='pt-BR'>")
            print("<head>")
            print("    <meta charset='UTF-8'>")
            print("    <meta name='viewport' content='width=device-width, initial-scale=1.0'>")
            print("    <title>Teste CGI Python</title>")
            print("    <link rel='stylesheet' href='./cgi/styles.css'>")
            print("</head>")
            print("<body>")
            print("    <div class='container'>")
            print(f"        <h1>Explorador de Arquivos - {html.escape(path)}</h1>")
            print("        <ul>")
            for name, rel_path in items:
                print(f"            <li><a href='?path={quote(rel_path)}'>{html.escape(name)}</a></li>")
            print("        </ul>")
            if path:
                print(f"        <a href='?path={quote(os.path.dirname(path))}' class='button'>Voltar</a>")
            else:
                print("        <a href='/' class='button'>Voltar</a>")
            print("    </div>")
            print("</body>")
            print("</html>")
    except:
        exit(1)

if __name__ == "__main__":
    main()