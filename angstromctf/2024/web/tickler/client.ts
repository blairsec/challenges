import { createTRPCClient, httpLink } from '@trpc/client'
import type { Router } from './server'

const client = createTRPCClient<Router>({ links: [
    httpLink({
        url: '/api',
        headers: () => {
            const username = localStorage.getItem('username')
            const password = localStorage.getItem('password')
            return { login: `${username}:${password}` }
        }
    })
] })

const checkLogin = async (): Promise<string | undefined> => {
    try {
        const result = await client.user.query()
        return result.user
    } catch {
        return undefined
    }
}

const getPicture = async (username: string): Promise<string> => {
    const params = new URLSearchParams({ username })
    const result = await fetch(`/picture?${params}`).then((res) => res.text())
    return result || '/default.png'
}

const getTickles = async (username: string): Promise<number | undefined> => {
    const result = await client.getTickles.query({ username })
    if (!result.success) {
        return undefined
    }
    return result.count
}

const routes: Record<string, () => void> = {
    '/': async () => {
        const p = document.querySelector('p') as HTMLParagraphElement
        const login = await checkLogin()
        if (login !== undefined) {
            const link = document.createElement('a')
            link.textContent = login
            link.href = '/profile'

            const text = document.createTextNode('Logged in as ')
            const period = document.createTextNode('.')

            p.appendChild(text)
            p.appendChild(link)
            p.appendChild(period)
        } else {
            const login = document.createElement('a')
            login.textContent = 'sign in'
            login.href = '/login'

            const register = document.createElement('a')
            register.textContent = 'register'
            register.href = '/register'

            p.append(
                document.createTextNode('Not logged in ('),
                login,
                document.createTextNode(' or '),
                register,
                document.createTextNode(').')
            )
        }
    },
    '/login': async () => {
        const form = document.querySelector('form') as HTMLFormElement
        const error = document.querySelector('p') as HTMLParagraphElement

        const query = new URLSearchParams(window.location.search)
        if (query.has('error')) {
            error.innerHTML = query.get('error') ?? ''
        }

        form.addEventListener('submit', async (event) => {
            event.preventDefault()
            const username = form.elements.namedItem('n') as HTMLInputElement
            const password = form.elements.namedItem('p') as HTMLInputElement

            const result = await client.doLogin.mutate({
                username: username.value,
                password: password.value
            })

            if (!result.success) {
                error.textContent = `Login failed. ${result.message}`
            } else {
                localStorage.setItem('username', username.value)
                localStorage.setItem('password', password.value)
                window.location.href = '/'
            }
        })
    },
    '/register': async () => {
        const suggest = document.querySelector('code') as HTMLElement
        const copy = document.querySelector('button') as HTMLButtonElement
        const password = document.querySelector(
            'input[name=p]'
        ) as HTMLInputElement
        const form = document.querySelector('form') as HTMLFormElement

        const random = crypto.randomUUID().replace(/-/g, '')
        suggest.textContent = random

        copy.addEventListener('click', () => {
            navigator.clipboard.writeText(random)
            password.value = random
        })

        form.addEventListener('submit', async (event) => {
            event.preventDefault()
            const username = form.elements.namedItem('n') as HTMLInputElement
            const password = form.elements.namedItem('p') as HTMLInputElement

            const result = await client.doRegister.mutate({
                username: username.value,
                password: password.value
            })

            if (!result.success) {
                suggest.textContent = result.message
            } else {
                localStorage.setItem('username', username.value)
                localStorage.setItem('password', password.value)
                window.location.href = '/'
            }
        })
    },
    '/profile': async () => {
        const login = await checkLogin()
        if (!login) {
            window.location.href = '/login?error=Not+logged+in.'
            return
        }

        const picture = document.querySelector('img') as HTMLImageElement
        const text = document.querySelector('.tickles') as HTMLParagraphElement
        const form = document.querySelector('form') as HTMLFormElement
        const error = document.querySelector('.error') as HTMLParagraphElement

        picture.src = await getPicture(login)
        text.textContent = `You have been tickled ${
            await getTickles(login) ?? 0
        } times.`

        form.addEventListener('submit', async (event) => {
            event.preventDefault()
            const url = form.elements.namedItem('image') as HTMLInputElement

            const result = await client.setPicture.mutate({ url: url.value })

            if (!result.success) {
                error.textContent = result.message
            } else {
                const data = await getPicture(login)
                picture.src = data
            }
        })
    },
    '/user': async () => {
        const query = new URLSearchParams(window.location.search)
        const username = query.get('name')
        const div = document.querySelector('.container') as HTMLDivElement
        const stack = document.querySelector('.stack') as HTMLDivElement
        const picture = document.querySelector('img') as HTMLImageElement
        const name = div.querySelector('.username') as HTMLParagraphElement
        const count = div.querySelector('.tickles') as HTMLParagraphElement
        const error = () => div.textContent = 'Not found.'

        if (username === null) return error()

        const tickles = await getTickles(username)
        if (tickles === undefined) return error()

        const image = await getPicture(username)
        picture.src = image

        name.textContent = `User ${username}.`
        count.textContent = `Tickled ${tickles} times.`

        const me = await checkLogin()
        if (me !== undefined && me !== username) {
            const button = document.createElement('button')
            button.textContent = 'Tickle!'

            button.addEventListener('click', async () => {
                const result = await client.doTickle.mutate({ username })
                if (result.success) {
                    window.location.reload()
                }
            })

            stack.appendChild(button)
        }
    },
}

window.addEventListener('load', () => {
    const element = document.querySelector(
        'meta[name=route]'
    ) as HTMLMetaElement | null
    const route = element?.content ?? '/'
    routes[route]?.()
})
