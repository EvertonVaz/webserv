# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    stress_test.py                                     :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/11 21:44:16 by Everton           #+#    #+#              #
#    Updated: 2024/11/11 22:10:33 by Everton          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

import asyncio
import aiohttp

res = []
async def send_request(session, url):
    async with session.get(url) as response:
        await response.read()
        if (response.status != 200):
            print('Request failed', response.status)
        res.append(response.status)

async def stress_test():
    url = 'http://localhost:8080/'
    num_requests = 1000
    tasks = []

    async with aiohttp.ClientSession() as session:
        for _ in range(num_requests):
            task = send_request(session, url)
            tasks.append(task)
        await asyncio.gather(*tasks)

asyncio.run(stress_test())
print('Total requests:', len(res))