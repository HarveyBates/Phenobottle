import Head from 'next/head'
import Image from 'next/image'
import styles from '../styles/Control.module.css'
import Link from 'next/link'

export default function Control() {
  return (
    <div className={styles.container}>
      <Head>
        <title>Phenobottle - Control Panel</title>
        <meta name="description" content="Phenobottle Control Panel" />
        <link rel="icon" href="/favicon.ico" />
      </Head>

      <main className={styles.main}>
        <h1 className={styles.title}>
          Control Panel
        </h1>
        <p className={styles.description}>
			Control Phenobottles from the browser.
        </p>
		 
		<hr className={styles.divider}></hr>
		
		<div className={styles.devices}>
			<div className={styles.connect}>
				<h2>Connected Phenobottles</h2>
				<button onClick="">Search &#x21bb;</button>
			</div>
			<p>Ensure device is powered on and operating as a server.</p>
			<table className={styles.table}>
			  <tr>
				  <th>Device Name</th>
				  <th>Device ID</th>
				  <th>Device Version</th>
				  <th>Treatment</th>
				  <th>Replicate</th>
				  <th>Status</th>
				  <th>Selected</th>
			  </tr>
			</table>
		</div>

      </main>

      <footer className={styles.footer}>
		  <p>HB Scientific</p>
      </footer>
    </div>
  )
}
