import Head from 'next/head'
import { Inter } from 'next/font/google'
import { useRef, useState } from 'react'

const inter = Inter({ subsets: ['latin'] })

export default function Home() {
  const [chatHistory, setChatHistory] = useState("")
  const [errorMessage, setErrorMessage] = useState("")
  const submitButton = useRef<HTMLInputElement>(null)

  async function submit(e: React.FormEvent<HTMLFormElement>) {
    e.preventDefault()

    const message = e.currentTarget.message.value
    setChatHistory(chatHistory + "\n" + `You: ${message}`)
    e.currentTarget.message.value = ""
    const history = e.currentTarget.history.value
    
    if (!!submitButton.current) {
      submitButton.current.disabled = true;
    }

    fetch("/api/chat", {
      method: "POST",
      headers: {
        "Content-Type": "application/json"
      },
      body: JSON.stringify({
        message: message,
        history: history
      })
    })
    .then(res => res.json())
    .then(data => {
      setChatHistory((state) => state + "\nMe: " + data.response)

      if (!!submitButton.current) {
        submitButton.current.disabled = false;
      }
    })
    .catch((e) => {
      if (!!submitButton.current) {
        submitButton.current.disabled = true;
      }
      setErrorMessage("An error occurred. Please refresh the page.")
    })
  }

  return (
    <>
      <Head>
        <meta charSet="UTF-8" />
        <meta httpEquiv="X-UA-Compatible" content="IE=edge" />
        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
        <title>Better Me</title>
      </Head>

      <main>
        <h1>Me, but Better</h1>
        <i className='error'>{errorMessage}</i>
        <div className="chat">
          <form onSubmit={submit}>
            <textarea className="chat-box" name="history" value={chatHistory} readOnly></textarea>

            <div className="message-me">
              <input type="text" name="message" placeholder="Talk to me" /><br />
              <input type="submit" ref={submitButton} value="Send" />
            </div>
          </form>
        </div>

      </main> 
    </>
  )
}
