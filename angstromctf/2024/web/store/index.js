const crypto = require('crypto')

const express = require('express')
const app = express()

const db = new require('better-sqlite3')(':memory:')

db.prepare(`
    CREATE TABLE items (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        name TEXT,
        detail TEXT
    );
`).run()

const flagTable = `flags${crypto.randomBytes(16).toString('hex')}`
db.prepare(`CREATE TABLE ${flagTable} ( flag TEXT);`).run()

db.prepare(`
    INSERT INTO ${flagTable} (flag)
    VALUES ('${process.env.FLAG ?? 'missing'}');
`).run()

const add = (item, detail) => {
    const query = db.prepare('INSERT INTO items (name, detail) VALUES (?, ?);')
    query.run(item, detail)
}

const search = (item) => {
    const query = db.prepare(`SELECT * FROM items WHERE name = '${item}';`)
    return query.all()
}

app.use(express.urlencoded({ extended: false }))

const page = (content) => `
    <link rel="stylesheet" href="/style.css">
    <div class="container">
        <h1>The \u00c3rmstrong Storefront Inventory</h1>
        <form action="/search" method="POST">
            <input
                type="text"
                name="item"
                placeholder="Otamatone, Echo dot, Razer Cynosa Chroma..."
            >
            <input type="submit" value="Search">
        </form>
        <script>
            const form = document.querySelector('form')
            form.addEventListener('submit', (event) => {
                const item = form.querySelector('input[name="item"]').value
                const allowed = new Set(
                    'abcdefghijklmnop' +
                    'qrstuvwxyzABCDEF' +
                    'GHIJKLMNOPQRSTUV' +
                    'WXYZ0123456789, '
                )
                if (item.split('').some((char) => !allowed.has(char))) {
                    alert('Invalid characters in search query.')
                    event.preventDefault()
                }
            })
        </script>
        ${content ?? ''}
    </div>
`

app.get('/', (_req, res) => {
    res.type('text/html').end(page())
})

const clean = (text) => text.replaceAll('<', '&lt;')
app.post('/search', (req, res) => {
    try {
        const results = search(req.body.item ?? '')
        res.type('text/html').end(page(`
            <table>
                <tr>
                    <th>Name</th>
                    <th>Details</th>
                </tr>
                ${results.map((result) => `
                    <tr>
                        <td>${clean(result.name)}</td>
                        <td>${clean(result.detail)}</td>
                    </tr>
                `).join('')}
            </table>
        `))
    } catch (error) {
        res.type('text/html').end(page('<p>An error occurred.</p>'))
    }
})


app.get('/style.css', (_req, res) => {
    res.type('text/css').end(`
        * {
          font-family: system-ui, -apple-system, BlinkMacSystemFont,
            'Segoe UI', Roboto, 'Helvetica Neue', sans-serif;
          box-sizing: border-box;
        }

        html,
        body {
          margin: 0;
        }

        .container {
          padding: 2rem;
          width: 90%;
          max-width: 900px;
          margin: auto;
        }

        input:not([type='submit']) {
          width: 100%;
          padding: 8px;
          margin: 8px 0;
        }

        textarea {
          width: 100%;
          padding: 8px;
          margin: 8px 0;
          resize: vertical;
          font-family: monospace;
        }

        input[type='submit'] {
          margin-bottom: 16px;
        }

        table {
            border-collapse: collapse;
        }

        tr {
            border-bottom: 1px solid #ccc;
        }

        th {
            text-align: left;
            padding: 0.5rem;
        }

        td {
            min-width: 20ch;
            padding: 0.5rem;
        }
    `)
})

add(
    'Otamatone',
    'A extremely serious synthesizer. Comes in a variety of colors.'
)

add(
    'Echo dot',
    'A smart speaker that can play music, make calls, and answer questions.'
)

add(
    'Razer Cynosa Chroma',
    'A gaming keyboard with customizable RGB lighting.'
)

app.listen(3000)
