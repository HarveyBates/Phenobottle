import Head from 'next/head'
import Image from 'next/image'
import styles from '../styles/Home.module.css'

export default function Home() {
  return (
    <div className={styles.container}>
      <Head>
        <title>Phenobottle - Home</title>
        <meta name="description" content="Phenobottle Homepage" />
        <link rel="icon" href="/favicon.ico" />
      </Head>

      <main className={styles.main}>
        <h1 className={styles.title}>
          Phenobottle UI
        </h1>

        <p className={styles.description}>
			An open-source photobioreactor for environmental simulation.
        </p>

        <div className={styles.grid}>
          <a href="/control" className={styles.card}>
            <h2>Control Panel &rarr;</h2>
            <p>Live control of Phenobottles from the web.</p>
          </a>

          <a href="https://nextjs.org/learn" className={styles.card}>
            <h2>Dashboard &rarr;</h2>
            <p>View Phenobottle metrics from the browser.</p>
          </a>

          <a
            href="https://github.com/vercel/next.js/tree/master/examples"
            className={styles.card}
          >
            <h2>Experiment &rarr;</h2>
            <p>Setup an experiment for a single or multiple Phenobottles.</p>
          </a>

          <a
            href="https://vercel.com/new?utm_source=create-next-app&utm_medium=default-template&utm_campaign=create-next-app"
            className={styles.card}
          >
            <h2>Settings &rarr;</h2>
            <p>Adjust default settings and features.</p>
          </a>
        </div>
      </main>

      <footer className={styles.footer}>
		  <p>HB Scientific</p>
      </footer>
    </div>
  )
}
