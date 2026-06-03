#pragma once
const char INDEX_HTML[] PROGMEM = R"rawhtml(<!DOCTYPE html> 
<html lang="es">
<head>
<meta charset="UTF-8"/>
<meta name="viewport" content="width=device-width,initial-scale=1"/>
<title>ESP32 Panel</title>
<link href="https://fonts.googleapis.com/css2?family=Share+Tech+Mono&family=Exo+2:wght@300;600;800&display=swap" rel="stylesheet"/>
<style>
  :root {
    --bg: #0a0c10;
    --surface: #0f1318;
    --border: #1e2a38;
    --accent: #00e5ff;
    --accent2: #ff4081;
    --accent3: #69ff47;
    --text: #c8d8e8;
    --muted: #4a6278;
    --mono: 'Share Tech Mono', monospace;
    --sans: 'Exo 2', sans-serif;
  }
  *, *::before, *::after { box-sizing: border-box; margin: 0; padding: 0; }
  body {
    background: var(--bg);
    color: var(--text);
    font-family: var(--sans);
    min-height: 100vh;
    overflow-x: hidden;
  }
  /* grid background */
  body::before {
    content: '';
    position: fixed; inset: 0; z-index: 0;
    background-image:
      linear-gradient(rgba(0,229,255,.03) 1px, transparent 1px),
      linear-gradient(90deg, rgba(0,229,255,.03) 1px, transparent 1px);
    background-size: 40px 40px;
    pointer-events: none;
  }
  .wrap {
    position: relative; z-index: 1;
    max-width: 820px;
    margin: 0 auto;
    padding: 32px 20px 60px;
  }
  /* Header */
  header {
    display: flex; align-items: center; gap: 16px;
    margin-bottom: 40px;
    border-bottom: 1px solid var(--border);
    padding-bottom: 24px;
  }
  .chip {
    width: 48px; height: 48px;
    border: 2px solid var(--accent);
    border-radius: 8px;
    display: grid; place-items: center;
    box-shadow: 0 0 18px rgba(0,229,255,.35);
    flex-shrink: 0;
  }
  .chip svg { width: 26px; height: 26px; fill: var(--accent); }
  .header-text h1 {
    font-size: 1.6rem; font-weight: 800; letter-spacing: .04em;
    color: #fff;
  }
  .header-text p {
    font-family: var(--mono); font-size: .72rem;
    color: var(--muted); margin-top: 2px;
  }
  .status-dot {
    margin-left: auto;
    display: flex; align-items: center; gap: 8px;
    font-family: var(--mono); font-size: .75rem; color: var(--accent3);
  }
  .dot {
    width: 8px; height: 8px; border-radius: 50%;
    background: var(--accent3);
    box-shadow: 0 0 8px var(--accent3);
    animation: pulse 2s ease-in-out infinite;
  }
  @keyframes pulse { 0%,100%{opacity:1} 50%{opacity:.4} }

  /* Cards */
  .cards { display: grid; gap: 16px; }
  .card {
    background: var(--surface);
    border: 1px solid var(--border);
    border-radius: 12px;
    padding: 20px 22px;
    transition: border-color .2s, box-shadow .2s;
    animation: fadeUp .4s ease both;
  }
  .card:hover { border-color: var(--accent); box-shadow: 0 0 24px rgba(0,229,255,.08); }
  @keyframes fadeUp { from{opacity:0;transform:translateY(12px)} to{opacity:1;transform:none} }
  .card:nth-child(1){animation-delay:.05s}
  .card:nth-child(2){animation-delay:.10s}
  .card:nth-child(3){animation-delay:.15s}
  .card:nth-child(4){animation-delay:.20s}

  .card-header {
    display: flex; align-items: center; gap: 10px;
    margin-bottom: 16px;
  }
  .method-badge {
    font-family: var(--mono); font-size: .65rem; font-weight: 600;
    padding: 2px 7px; border-radius: 4px;
    background: rgba(0,229,255,.12); color: var(--accent);
    border: 1px solid rgba(0,229,255,.25);
  }
  .route {
    font-family: var(--mono); font-size: .95rem; color: #fff;
  }
  .desc {
    font-size: .78rem; color: var(--muted); margin-left: auto;
  }
  .fields { display: flex; flex-wrap: wrap; gap: 8px; align-items: center; }
  .field-wrap { display: flex; flex-direction: column; gap: 4px; flex: 1; min-width: 160px; }
  .field-label { font-family: var(--mono); font-size: .65rem; color: var(--muted); }
  input[type=text] {
    background: #070a0d;
    border: 1px solid var(--border);
    border-radius: 6px;
    color: var(--text);
    font-family: var(--mono);
    font-size: .82rem;
    padding: 8px 10px;
    width: 100%;
    transition: border-color .2s, box-shadow .2s;
    outline: none;
  }
  input[type=text]:focus {
    border-color: var(--accent);
    box-shadow: 0 0 0 2px rgba(0,229,255,.12);
  }
  input[type=text]::placeholder { color: #2e4255; }
  .btn {
    font-family: var(--sans); font-size: .8rem; font-weight: 600;
    letter-spacing: .06em; text-transform: uppercase;
    padding: 8px 18px; border-radius: 6px; border: none;
    cursor: pointer; transition: all .18s; flex-shrink: 0;
    align-self: flex-end;
    position: relative; overflow: hidden;
  }
  .btn-cyan {
    background: rgba(0,229,255,.12);
    color: var(--accent);
    border: 1px solid rgba(0,229,255,.3);
  }
  .btn-cyan:hover { background: rgba(0,229,255,.22); box-shadow: 0 0 16px rgba(0,229,255,.2); }
  .btn-pink {
    background: rgba(255,64,129,.12);
    color: var(--accent2);
    border: 1px solid rgba(255,64,129,.3);
  }
  .btn-pink:hover { background: rgba(255,64,129,.22); box-shadow: 0 0 16px rgba(255,64,129,.2); }
  .btn:disabled { opacity: .4; cursor: not-allowed; }
  .btn .spinner {
    display: none; width: 12px; height: 12px;
    border: 2px solid currentColor; border-top-color: transparent;
    border-radius: 50%; animation: spin .6s linear infinite;
    vertical-align: middle; margin-right: 4px;
  }
  .btn.loading .spinner { display: inline-block; }
  @keyframes spin { to{transform:rotate(360deg)} }

  /* Response box */
  .resp-box {
    margin-top: 12px;
    border-radius: 8px;
    overflow: hidden;
    display: none;
    animation: fadeIn .2s ease;
  }
  @keyframes fadeIn { from{opacity:0} to{opacity:1} }
  .resp-box.visible { display: block; }
  .resp-header {
    display: flex; align-items: center; justify-content: space-between;
    padding: 6px 12px;
    font-family: var(--mono); font-size: .68rem;
  }
  .resp-header.ok { background: rgba(105,255,71,.1); color: var(--accent3); }
  .resp-header.err { background: rgba(255,64,129,.1); color: var(--accent2); }
  .resp-body {
    background: #070a0d;
    border: 1px solid var(--border);
    border-top: none;
    padding: 10px 12px;
    font-family: var(--mono); font-size: .78rem;
    color: var(--text);
    word-break: break-all;
    white-space: pre-wrap;
    max-height: 120px; overflow-y: auto;
  }

  /* Log */
  .log-section { margin-top: 32px; }
  .log-title {
    font-family: var(--mono); font-size: .72rem; color: var(--muted);
    margin-bottom: 8px; letter-spacing: .08em;
  }
  .log {
    background: #070a0d;
    border: 1px solid var(--border);
    border-radius: 8px;
    padding: 12px;
    height: 140px; overflow-y: auto;
    font-family: var(--mono); font-size: .72rem;
    color: var(--muted);
  }
  .log-entry { padding: 2px 0; }
  .log-entry .ts { color: #2e4255; margin-right: 8px; }
  .log-entry .ok-tag { color: var(--accent3); }
  .log-entry .err-tag { color: var(--accent2); }
  .log-entry .url-tag { color: var(--accent); }
  .log::-webkit-scrollbar { width: 4px; }
  .log::-webkit-scrollbar-thumb { background: var(--border); border-radius: 2px; }
</style>
</head>
<body>
<div class="wrap">
  <header>
    <div class="chip">
      <svg viewBox="0 0 24 24" xmlns="http://www.w3.org/2000/svg">
        <path d="M9 3H7v2H5v2H3v2h2V7h2V5h2V3zm8 0h-2v2h2v2h2v2h2V7h-2V5h-2V3zM9 19H7v-2H5v-2H3v2h2v2h2v2h2v-2zm8 0h-2v2h2v-2h2v-2h2v-2h-2v2h-2v2zm-5-6h2v2h-2v-2zm0-4h2v2h-2V9zm-4 4h2v-2H8v2zm8 0h2v-2h-2v2z"/>
      </svg>
    </div>
    <div class="header-text">
      <h1>ESP32 Control Panel</h1>
      <p>192.168.4.1 &nbsp;|&nbsp; WebServer v1.0 &nbsp;|&nbsp; Hidden_Network</p>
    </div>
    <div class="status-dot"><div class="dot"></div>ONLINE</div>
  </header>

  <div class="cards">

    <!-- /mp -->
    <div class="card">
      <div class="card-header">
        <span class="method-badge">GET</span>
        <span class="route">/mp</span>
        <span class="desc">Genera Magic Packet (no envia)</span>
      </div>
      <div class="fields">
        <div class="field-wrap">
          <span class="field-label">MAC ADDRESS</span>
          <input type="text" id="mp1" placeholder="4C:34:88:63:35:5D"/>
        </div>
        <button class="btn btn-cyan" onclick="call('/mp','mp',{mp:'mp1'},'r1')">
          <span class="spinner"></span>Generar
        </button>
      </div>
      <div class="resp-box" id="r1"></div>
    </div>

    <!-- /sendMP -->
    <div class="card">
      <div class="card-header">
        <span class="method-badge">GET</span>
        <span class="route">/sendMP</span>
        <span class="desc">Envia Wake-on-LAN via UDP broadcast</span>
      </div>
      <div class="fields">
        <div class="field-wrap">
          <span class="field-label">MAC ADDRESS</span>
          <input type="text" id="mp2" placeholder="4C:34:88:63:35:5D"/>
        </div>
        <button class="btn btn-cyan" onclick="call('/sendMP','sendmp',{mp:'mp2'},'r2')">
          <span class="spinner"></span>Enviar WoL
        </button>
      </div>
      <div class="resp-box" id="r2"></div>
    </div>

    <!-- /printf -->
    <div class="card">
      <div class="card-header">
        <span class="method-badge">GET</span>
        <span class="route">/printf</span>
        <span class="desc">Envia mensaje TCP al equipo remoto</span>
      </div>
      <div class="fields">
        <div class="field-wrap">
          <span class="field-label">IP DESTINO</span>
          <input type="text" id="pf_ip" placeholder="192.168.4.2"/>
        </div>
        <div class="field-wrap">
          <span class="field-label">MENSAJE</span>
          <input type="text" id="pf_msg" placeholder="HolaMundo"/>
        </div>
        <button class="btn btn-cyan" onclick="call('/printf','printf',{ip:'pf_ip',msg:'pf_msg'},'r3')">
          <span class="spinner"></span>Enviar
        </button>
      </div>
      <div class="resp-box" id="r3"></div>
    </div>

    <!-- /suspendPC -->
    <div class="card">
      <div class="card-header">
        <span class="method-badge">GET</span>
        <span class="route">/suspendPC</span>
        <span class="desc">Suspende el equipo remoto (1 min)</span>
      </div>
      <div class="fields">
        <div class="field-wrap">
          <span class="field-label">IP DESTINO</span>
          <input type="text" id="susp_ip" placeholder="192.168.4.2"/>
        </div>
        <button class="btn btn-pink" onclick="call('/suspendPC','suspend',{ip:'susp_ip'},'r4')">
          <span class="spinner"></span>Suspender PC
        </button>
      </div>
      <div class="resp-box" id="r4"></div>
    </div>

  </div><!-- /cards -->

  <div class="log-section">
    <div class="log-title">// REQUEST LOG</div>
    <div class="log" id="log"></div>
  </div>
</div>

<script>
function ts() {
  const d = new Date();
  return d.toTimeString().slice(0,8);
}
function addLog(url, ok, msg) {
  const log = document.getElementById('log');
  const e = document.createElement('div');
  e.className = 'log-entry';
  e.innerHTML = '<span class="ts">' + ts() + '</span>'
    + (ok ? '<span class="ok-tag">[OK]</span>' : '<span class="err-tag">[ERR]</span>')
    + ' <span class="url-tag">' + url + '</span>'
    + ' &mdash; ' + msg.substring(0, 80) + (msg.length > 80 ? '...' : '');
  log.appendChild(e);
  log.scrollTop = log.scrollHeight;
}
function call(path, _name, paramMap, respId) {
  const params = new URLSearchParams();
  let valid = true;
  for (const [key, inputId] of Object.entries(paramMap)) {
    const val = document.getElementById(inputId).value.trim();
    if (!val) { 
      document.getElementById(inputId).focus();
      document.getElementById(inputId).style.borderColor = '#ff4081';
      setTimeout(() => document.getElementById(inputId).style.borderColor = '', 1200);
      valid = false; break;
    }
    params.append(key, val);
  }
  if (!valid) return;

  const url = path + '?' + params.toString();
  const box = document.getElementById(respId);
  const btn = event.currentTarget;
  btn.classList.add('loading');
  btn.disabled = true;

  fetch(url)
    .then(r => {
      const ok = r.ok;
      return r.text().then(t => ({ ok, t, status: r.status }));
    })
    .then(({ ok, t, status }) => {
      showResp(box, ok, status, t);
      addLog(url, ok, t);
    })
    .catch(err => {
      showResp(box, false, 0, 'Network error: ' + err.message);
      addLog(url, false, err.message);
    })
    .finally(() => {
      btn.classList.remove('loading');
      btn.disabled = false;
    });
}
function showResp(box, ok, status, text) {
  box.classList.add('visible');
  box.innerHTML =
    '<div class="resp-header ' + (ok ? 'ok' : 'err') + '">'
    + '<span>' + (ok ? '&#10003; OK' : '&#10007; ERROR') + '</span>'
    + '<span>HTTP ' + (status || '—') + '</span>'
    + '</div>'
    + '<div class="resp-body">' + escHtml(text) + '</div>';
}
function escHtml(s) {
  return s.replace(/&/g,'&amp;').replace(/</g,'&lt;').replace(/>/g,'&gt;');
}
// Initial log entry
addLog('/', true, 'Panel cargado correctamente');
</script>
</body>
</html>)rawhtml";