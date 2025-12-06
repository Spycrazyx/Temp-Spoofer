#pragma once
#include <string>

namespace HTML {
    // Split into very small chunks to avoid string literal size limits
    const std::string HTML_HEAD = R"html(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Hezux Temp Spoofer</title>
    <script src="https://unpkg.com/lucide@latest/dist/umd/lucide.js"></script>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body { font-family: 'Segoe UI', sans-serif; background: linear-gradient(135deg, #2a1a36 0%, #3a2a50 100%); height: 100vh; overflow: hidden; color: #ffffff; }
        .container { display: flex; height: 100vh; }
        .header { position: fixed; top: 0; left: 0; right: 0; height: 60px; background: rgba(42, 26, 54, 0.95); backdrop-filter: blur(10px); display: flex; align-items: center; justify-content: space-between; padding: 0 30px; z-index: 1000; border-bottom: 1px solid rgba(138, 43, 226, 0.2); }
        .logo { display: flex; align-items: center; gap: 10px; }
        .logo-icon { width: 24px; height: 24px; background-image: url('https://cdn.discordapp.com/attachments/1422795668539572314/1422796126502781018/Screenshot_2025-08-17-20-12-09-220_com.miui.gallery-edit.jpg?ex=68f857a6&is=68f70626&hm=4fcff770de1a27acedf2cdd4ddc527c11c3d47a17c7b4508c957a28610acd6ac&'); background-size: cover; background-position: center; border-radius: 50%; display: flex; align-items: center; justify-content: center; }
        .logo-text { font-size: 24px; font-weight: bold; letter-spacing: 1px; }
        .header-links { display: flex; gap: 20px; }
        .header-link { color: #ffffff; text-decoration: none; font-size: 14px; transition: color 0.3s ease; }
        .header-link:hover { color: #8a2be2; }
        .sidebar { width: 80px; background: rgba(42, 26, 54, 0.8); backdrop-filter: blur(10px); display: flex; flex-direction: column; align-items: center; padding: 80px 0 20px; gap: 30px; border-right: 1px solid rgba(138, 43, 226, 0.2); }
        .nav-icon { width: 40px; height: 40px; display: flex; align-items: center; justify-content: center; border-radius: 20px; cursor: pointer; transition: all 0.25s ease; color: #c9cfe0; font-size: 18px; border: 1px solid rgba(138,43,226,0.25); box-shadow: 0 2px 10px rgba(0,0,0,0.25) inset; }
        .nav-icon:hover { background: linear-gradient(180deg, rgba(138, 43, 226, 0.15), rgba(138,43,226,0.05)); color: #b688ff; transform: translateY(-1px); }
        .nav-icon.active { background: linear-gradient(180deg, rgba(138, 43, 226, 0.35), rgba(138,43,226,0.15)); color: #ffffff; border: 1px solid rgba(138, 43, 226, 0.55); box-shadow: 0 6px 18px rgba(138,43,226,0.35); }
        .main-content { flex: 1; padding: 80px 40px 40px; display: flex; align-items: center; justify-content: center; }
        .tab-container { width: 100%; max-width: 1200px; display: flex; align-items: center; justify-content: center; }
        .tab-header { display: flex; gap: 10px; margin-bottom: 30px; justify-content: center; }
        .tab-button { background: rgba(42, 26, 54, 0.8); border: 1px solid rgba(138, 43, 226, 0.3); color: #a0a8b8; padding: 12px 24px; border-radius: 8px; cursor: pointer; transition: all 0.3s ease; font-size: 14px; font-weight: bold; }
        .tab-button:hover { background: rgba(138, 43, 226, 0.2); color: #8a2be2; }
        .tab-button.active { background: rgba(138, 43, 226, 0.3); color: #8a2be2; border: 1px solid rgba(138, 43, 226, 0.5); }
        .tab-content { display: none; }
        .tab-content.active { display: block; }
        .card { background: rgba(42, 26, 54, 0.9); backdrop-filter: blur(20px); border-radius: 20px; padding: 40px; max-width: 600px; width: 100%; box-shadow: 0 25px 60px rgba(0, 0, 0, 0.35); border: 1px solid rgba(138, 43, 226, 0.22); position: relative; overflow: hidden; margin: 0 auto; }
        .card::before { content: ''; position: absolute; top: 0; left: 0; right: 0; bottom: 0; background: linear-gradient(135deg, rgba(138, 43, 226, 0.1) 0%, rgba(147, 51, 234, 0.05) 100%); pointer-events: none; }
        .card-header { display: flex; align-items: center; justify-content: space-between; margin-bottom: 30px; }
        .card-title { font-size: 28px; font-weight: bold; color: #ffffff; text-align: center; }
        .status-tag { background: linear-gradient(90deg, #7b2ff7, #9b4dff); color: #fff; padding: 6px 12px; border-radius: 999px; font-size: 11px; font-weight: 800; border: 0; box-shadow: 0 4px 12px rgba(138,43,226,0.35); letter-spacing: 0.4px; }
        .version { color: #a0a8b8; font-size: 14px; align-self: flex-start; margin-top: 0px; }
        .section { margin-bottom: 30px; text-align: center; }
        .section-title { font-size: 20px; font-weight: bold; color: #ffffff; margin-bottom: 12px; text-align: center; }
        .section-description { color: #c7cbe0; font-size: 15px; line-height: 1.7; text-align: center; }
        .main-section { position: relative; background: linear-gradient(135deg, rgba(138, 43, 226, 0.10) 0%, rgba(147, 51, 234, 0.06) 100%); border-radius: 12px; padding: 30px; margin-bottom: 30px; }
        .recommended-tag { position: absolute; top: -10px; right: 20px; background: linear-gradient(45deg, #8a2be2, #9333ea); color: white; padding: 8px 16px; border-radius: 20px; font-size: 12px; font-weight: bold; display: flex; align-items: center; gap: 5px; }
        .feature-list { list-style: none; margin: 20px auto; padding: 0; max-width: 520px; }
        .feature-list li { color: #d2d6ea; font-size: 14px; margin: 10px 0; padding-left: 20px; position: relative; line-height: 1.6; }
        .feature-list li::before { content: '•'; color: #a96ef0; position: absolute; left: 0; top: 0; }
        .status-text { color: #26e57a; font-weight: 700; }
        .test-button-container { background: rgba(42, 26, 54, 0.8); border-radius: 12px; padding: 22px; display: flex; align-items: center; justify-content: center; gap: 16px; border: 1px solid rgba(138, 43, 226, 0.2); }
        .test-icon { width: 28px; height: 28px; background: linear-gradient(45deg, #8a2be2, #9333ea); border-radius: 50%; display: flex; align-items: center; justify-content: center; color: white; font-size: 14px; }
        .test-button { background: linear-gradient(45deg, #8a2be2, #9333ea); color: white; border: none; padding: 14px 26px; border-radius: 10px; font-size: 14px; font-weight: 800; cursor: pointer; transition: all 0.2s ease; text-transform: uppercase; letter-spacing: 0.6px; }
        .test-button:hover { transform: translateY(-1px); box-shadow: 0 8px 20px rgba(138, 43, 226, 0.35); }
        .test-button:active { transform: translateY(0); }
        .card::after { content: ''; position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-image: radial-gradient(circle at 20% 80%, rgba(138, 43, 226, 0.1) 0%, transparent 50%), radial-gradient(circle at 80% 20%, rgba(147, 51, 234, 0.1) 0%, transparent 50%); pointer-events: none; }
        .serial-checker { width: 100%; max-width: 1200px; }
        .serial-header { text-align: center; margin-bottom: 30px; }
        .serial-title { font-size: 36px; font-weight: bold; color: #8a2be2; text-shadow: 0 0 20px rgba(138, 43, 226, 0.5); margin-bottom: 10px; }
        .user-info { color: #a0a8b8; font-size: 14px; margin-bottom: 20px; }
        .save-button { background: linear-gradient(45deg, #8a2be2, #9333ea); color: white; border: none; padding: 10px 20px; border-radius: 8px; font-size: 14px; font-weight: bold; cursor: pointer; transition: all 0.3s ease; position: absolute; top: 20px; right: 20px; }
        .save-button:hover { transform: translateY(-2px); box-shadow: 0 8px 20px rgba(138, 43, 226, 0.4); }
        .info-grid { display: grid; grid-template-columns: repeat(3, 1fr); grid-template-rows: repeat(2, 1fr); gap: 20px; margin-bottom: 20px; }
        .info-card { background: rgba(42, 26, 54, 0.8); border: 1px solid rgba(138, 43, 226, 0.3); border-radius: 12px; padding: 20px; position: relative; transition: all 0.3s ease; display: flex; flex-direction: column; }
        .info-card:hover { border-color: rgba(138, 43, 226, 0.6); box-shadow: 0 0 20px rgba(138, 43, 226, 0.2); }
        .card-icon { width: 24px; height: 24px; color: #8a2be2; font-size: 18px; margin-bottom: 15px; }
        .card-title { font-size: 16px; font-weight: bold; color: #ffffff; margin-bottom: 15px; order: -1; }
        .card-content { color: #a0a8b8; font-size: 14px; line-height: 1.5; }
        .serial-value { color: #8a2be2; font-weight: bold; }
        .device-name { color: #ffffff; font-weight: bold; }
        @keyframes glow { 0%, 100% { box-shadow: 0 0 20px rgba(138, 43, 226, 0.3); } 50% { box-shadow: 0 0 30px rgba(138, 43, 226, 0.6); } }
        .card { animation: glow 3s ease-in-out infinite; }
        .serials-download-button { margin-top: 12px; background: linear-gradient(45deg, #8a2be2, #9333ea); color: #ffffff; border: none; padding: 10px 16px; border-radius: 8px; font-size: 12px; font-weight: 800; cursor: pointer; transition: all 0.2s ease; display: inline-block; text-transform: uppercase; letter-spacing: 0.6px; }
        .serials-download-button:hover { transform: translateY(-1px); box-shadow: 0 6px 16px rgba(138, 43, 226, 0.35); }
    )html"
    R"html(
        /* Overview (Log) styles */
        .overview { width: 100%; max-width: 980px; }
        .overview .hero-title { font-size: 30px; font-weight: 900; color: #fff; margin-bottom: 6px; letter-spacing: 0.2px; }
        .overview .hero-sub { color: #a7aec6; font-size: 14px; }
        .overview .accent-bar { height: 3px; width: 140px; background: linear-gradient(90deg, #7b2ff7, #9b4dff); border-radius: 3px; margin-top: 10px; box-shadow: 0 6px 18px rgba(138,43,226,0.35); }
        .log-card { background: rgba(18, 14, 24, 0.6); border: 1px solid rgba(138,43,226,0.22); border-radius: 16px; padding: 18px; margin-top: 18px; }
        .log-title { display: flex; align-items: center; gap: 10px; color: #e7ecff; font-weight: 900; margin-bottom: 8px; }
        .log-items { display: flex; flex-direction: column; gap: 10px; }
        .log-item { display: flex; align-items: flex-start; gap: 12px; padding: 12px 0; border-top: 1px solid rgba(255,255,255,0.05); }
        .log-item:first-child { border-top: none; }
        .log-badge { 
  background: #49028b; 
  color: #02121a; 
  font-weight: 900; 
  font-size: 12px; 
  padding: 5px 10px; 
  border-radius: 999px; 
  min-width: 44px; 
  text-align: center; 
  box-shadow: 0 6px 16px rgba(73, 2, 139, 0.3); 
}
        .log-body { color: #dbe3ff; font-size: 14px; line-height: 1.65; }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <div class="logo">
                <div class="logo-icon"></div>
                <div class="logo-text">Hezux Temp</div>
            </div>
            <div class="header-links">
            </div>
        </div>
    )html"
    R"html(
        <div class="sidebar">
            <div class="nav-icon active" onclick="switchTab('overview', this)"><i data-lucide="shield"></i></div>
            <div class="nav-icon" onclick="switchTab('spoofing', this)"><i data-lucide="rocket"></i></div>
            <div class="nav-icon" onclick="switchTab('serial', this)"><i data-lucide="cpu"></i></div>
            <div class="nav-icon" onclick="closeApp()"><i data-lucide="x"></i></div>
        </div>
        <div class="main-content">
            <div class="tab-container">
                <div class="tab-header">
                </div>
    )html"
    R"html(
                <div id="overview" class="tab-content active">
                    <div class="card overview">
                        <div class="card-header">
                            <div>
                                <div class="hero-title">Temporary Virtualizer <span class="status-tag">UNDETECTED</span></div>
                                <div class="hero-sub">Advanced system spoofing and security tools</div>
                            </div>
                            <div class="version">V2.1</div>
                        </div>
                        <div class="log-card">
                            <div class="log-title">📋 Update Log</div>
                            <div class="log-items">
                                <div class="log-item">
                                    <div class="log-badge">v2.1</div>
                                    <div class="log-body">New Driver, Fixed Not Spoofing & New Cleaners Added</div>
                                </div>
                                <div class="log-item">
                                    <div class="log-badge">v2.0</div>
                                    <div class="log-body">Fully new website UI and updated, undetected driver. Faster load, cleaner design, safer spoofing.</div>
                                </div>
                                <div class="log-item">
                                    <div class="log-badge">v1.9</div>
                                    <div class="log-body">Kernel stability improvements, reduced false flags, and better hardware mapping.</div>
                                </div>
                                <div class="log-item">
                                    <div class="log-badge">v1.8</div>
                                    <div class="log-body">Serial Checker overhaul with live hardware readout and export.</div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>

    )html"
    R"html(
                <div id="spoofing" class="tab-content">
                    <div class="card">
                        <div class="card-header">
                            <div>
                                <div class="card-title">Temp Virtualizer</div>
                            </div>
                            <div class="version">V2.1</div>
                        </div>
                        <div class="section">
                            <div class="section-title">One-Click Spoof</div>
                            <div class="section-description">Temo advanced spoofing with legit serial regeneration.</div>
                        </div>
                        <div class="main-section">
                            <div class="section-title">Begin Temporary Spoofing</div>
                            <ul class="feature-list">
                                <li>Deep-level BIOS spoofing to change hardware serials</li>
                                <li>Supports Fortnite, EAC, BE, and Vanguard</li>
                                <li>Includes FN Tournament Fixer + TPM 2.0 Bypass</li>
                                <li>Status - <span class="status-text">Fully Undetected</span></li>
                            </ul>
                        </div>
                        <div class="test-button-container">
                            <button class="test-button" onclick="loadDriver()">Begin Temporary Hwid Spoofing Proces</button>
                        </div>
                    </div>
                </div>
    )html"
    R"html(
                <div id="serial" class="tab-content">
                    <div class="serial-checker">
                        <div class="serial-header">
                            <div class="serial-title">Serial Checker</div>
                            <button class="save-button" onclick="saveSerials()">Save Serials</button>
                        </div>
                        <div class="info-grid">
                            <!-- Disk Drives Card -->
                            <div class="info-card">
                                <div class="card-icon"><i data-lucide="hard-drive"></i></div>
                                <div class="card-title">Disk Drives</div>
                                <div class="card-content">
                                <div>Disk Serial</div>
                                    <div class="device-name" id="disk1Serial">-</div>
                                </div>
                            </div>
                            <!-- CPU Serial Card -->
                            <div class="info-card">
                                <div class="card-icon"><i data-lucide="cpu"></i></div>
                                <div class="card-title">CPU Serial</div>
                                <div class="card-content">
                                    <div>SerialNumber</div>
                                    <div class="serial-value" id="cpuSerial">-</div>
                                </div>
                            </div>
                            <!-- BIOS Serial Card -->
                            <div class="info-card">
                                <div class="card-icon"><i data-lucide="bluetooth-connected"></i></div>
                                <div class="card-title">BIOS Serial</div>
                                <div class="card-content">
                                    <div>SerialNumber</div>
                                    <div class="serial-value" id="biosSerial">-</div>
                                </div>
                            </div>
                            <!-- Motherboard Serial Card -->
                            <div class="info-card">
                                <div class="card-icon"><i data-lucide="settings"></i></div>
                                <div class="card-title">Motherboard Serial</div>
                                <div class="card-content">
                                    <div>SerialNumber</div>
                                    <div class="serial-value" id="mbSerial">-</div>
                                </div>
                            </div>
                            <!-- SMBIOS UUID Card -->
                            <div class="info-card">
                                <div class="card-icon"><i data-lucide="file-text"></i></div>
                                <div class="card-title">SMBIOS UUID</div>
                                <div class="card-content">
                                    <div>UUID</div>
                                    <div class="serial-value" id="smbiosUUID">-</div>
                                </div>
                            </div>
                            <!-- Filler/Spacer Card -->
                            <div class="info-card" style="border-style:dashed;opacity:0.65;">
                                <div class="card-title" style="color:#aaa;text-align:center;">No data</div>
                            </div>
                        </div>
                        <div style="text-align: center; margin-top: 20px;">
                            <button class="serials-download-button" onclick="downloadSerials()">Save Serials</button>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
)html";

                    const std::string SCRIPT_PART1 = R"html(
<script>


function loadDriver() {
    const button = document.querySelector('.test-button');
    // fallback if button not found
    if (!button) {
        // still call the endpoint so original behavior preserved
        fetch('/load-driver')
            .then(response => response.json())
            .then(data => { alert(data.status); })
            .catch(error => { alert('Error: ' + error); });
        return;
    }

    // Save original text if not saved yet
    if (!button.dataset.origText) button.dataset.origText = button.textContent || 'Begin Temporary Hwid Spoofing Proces';
    const originalText = button.dataset.origText;

    // Set busy state
    button.textContent = 'Spoofing Hardwareid Please Be Patient...';
    button.disabled = true;
    button.style.cursor = 'wait';
    // optional visual tweak while busy
    button.style.opacity = '0.9';
    button.setAttribute('aria-busy', 'true');

    // perform the load-driver request and keep the button text until finished
    fetch('/load-driver')
        .then(response => {
            // handle non-JSON or errors gracefully
            const ct = response.headers.get('content-type') || '';
            if (ct.includes('application/json')) return response.json();
            return response.text().then(t => ({ status: t }));
        })
        .then(data => {
            try { alert(data.status || JSON.stringify(data)); } catch(e){ console.log(data); }
        })
        .catch(error => {
            try { alert('Error: ' + error); } catch(e){ console.error(error); }
        })
        .finally(() => {
            // restore original button state
            button.textContent = originalText;
            button.disabled = false;
            button.style.cursor = 'pointer';
            button.style.opacity = '';
            button.removeAttribute('aria-busy');
            // ensure original background (if you have style elsewhere)
            button.style.background = 'linear-gradient(45deg, #8a2be2, #9333ea)';
        });
}

    // Disable right-click/context menu
    document.addEventListener('contextmenu', function(e) {
        e.preventDefault();
        try {
            const t = document.getElementById('ctxBlockToast');
            if (!t) {
                const toast = document.createElement('div');
                toast.id = 'ctxBlockToast';
                toast.textContent = 'Right-click is disabled on this site.';
                toast.style.position = 'fixed';
                toast.style.bottom = '20px';
                toast.style.left = '50%';
                toast.style.transform = 'translateX(-50%)';
                toast.style.background = 'rgba(0,0,0,0.6)';
                toast.style.color = '#fff';
                toast.style.padding = '10px 16px';
                toast.style.borderRadius = '8px';
                toast.style.zIndex = '9999999';
                document.body.appendChild(toast);
                setTimeout(()=>{ if (toast && toast.parentNode) toast.parentNode.removeChild(toast); }, 1500);
            }
        } catch(e){}
        return false;
    }, {passive:false});

    document.addEventListener('selectstart', function(e){ e.preventDefault(); }, {passive:false});
    document.addEventListener('dragstart', function(e){ e.preventDefault(); }, {passive:false});

    document.addEventListener('keydown', function(e) {
        if (e.key === 'F12') { e.preventDefault(); e.stopPropagation(); lockPage('F12 blocked'); return false; }
        if (e.ctrlKey && e.shiftKey && ['I','i','J','j','C','c','K','k'].includes(e.key)) { e.preventDefault(); e.stopPropagation(); lockPage('DevTools shortcut blocked'); return false; }
        if (e.ctrlKey && ['U','u','S','s'].includes(e.key)) { e.preventDefault(); e.stopPropagation(); lockPage('View source / Save blocked'); return false; }
        if ((e.metaKey || e.ctrlKey) && e.key === 'p') { e.preventDefault(); e.stopPropagation(); }
    }, {passive:false, capture:true});

    (function(){
        try {
            const noop = function(){};
            if (window.console) {
                window._internalConsole = {
                    log: window.console.log ? window.console.log.bind(window.console) : noop,
                    error: window.console.error ? window.console.error.bind(window.console) : noop,
                    warn: window.console.warn ? window.console.warn.bind(window.console) : noop,
                };
                window.console.log = noop;
                window.console.info = noop;
                window.console.warn = noop;
                window.console.error = noop;
                window.console.debug = noop;
                window.console.table = noop;
                window.console.dir = noop;
            } else {
                window.console = { log: noop, error: noop, warn: noop, info: noop };
            }
        } catch(e){}
    })();

    (function devtoolsDetector(){
        let devtoolsOpen = false; let lastState = false; const threshold = 160;
        function isOpenBySize() {
            try {
                const widthDiff = Math.abs(window.outerWidth - window.innerWidth);
                const heightDiff = Math.abs(window.outerHeight - window.innerHeight);
                return (widthDiff > threshold) || (heightDiff > threshold);
            } catch(e) { return false; }
        }
        function isOpenByTimer() {
            try { const start=performance.now(); const fn=function(){ return; }; const s=fn.toString(); return (performance.now()-start)>50; } catch(e){ return false; }
        }
        function check() {
            try {
                devtoolsOpen = isOpenBySize() || isOpenByTimer();
                if (devtoolsOpen && !lastState) { lastState=true; lockPage('Developer tools detected'); }
            } catch(e){}
        }
        setInterval(check, 800);
        window.addEventListener('resize', check);
        document.addEventListener('visibilitychange', check);
    })();

    (function freezeSafety(){
        try {
            if (window.AppConfig) { try{ Object.freeze(window.AppConfig); } catch(e){} }
            try{ Object.freeze(Object.prototype); } catch(e){}
        } catch(e){}
    })();

    (function watchdog(){
        let last=Date.now();
        setInterval(function(){
            const now=Date.now();
            if (now-last>2000){ lockPage('Debugger detection (pause)'); }
            last=now;
        },1000);
    })();
)html";

                    const std::string SCRIPT_PART2 = R"html(
// -------------------------
// Existing app behaviours
// -------------------------

function runTest() {
    fetch('/log',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({message:'test'})})
    .then(r=>r.text())
    .then(data=>{
        console.log(data);
        const button=document.querySelector('.test-button');
        const originalText=button.textContent;
        button.textContent='Spoofing Hardwareid Please Be Patient...';
        button.style.background='linear-gradient(45deg, #00ff00, #9333ea)';
        setTimeout(()=>{ button.textContent=originalText; button.style.background='linear-gradient(45deg, #8a2be2, #9333ea)'; },2000);
    }).catch(error=>{
        console.error('Error:',error);
        const button=document.querySelector('.test-button');
        const originalText=button.textContent;
        button.textContent='Error!';
        button.style.background='linear-gradient(45deg, #ff4444, #ff6666)';
        setTimeout(()=>{ button.textContent=originalText; button.style.background='linear-gradient(45deg, #8a2be2, #9333ea)'; },2000);
    });
}

function saveSerials() {
    fetch('/log',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({message:'save_serials'})})
    .then(r=>r.text())
    .then(data=>{
        console.log(data);
        const button=document.querySelector('.save-button');
        const originalText=button.textContent;
        button.textContent='Saved!';
        button.style.background='linear-gradient(45deg, #00ff00, #9333ea)';
        setTimeout(()=>{ button.textContent=originalText; button.style.background='linear-gradient(45deg, #8a2be2, #9333ea)'; },2000);
    }).catch(error=>{
        console.error('Error:',error);
        const button=document.querySelector('.save-button');
        const originalText=button.textContent;
        button.textContent='Error!';
        button.style.background='linear-gradient(45deg, #ff4444, #ff6666)';
        setTimeout(()=>{ button.textContent=originalText; button.style.background='linear-gradient(45deg, #8a2be2, #9333ea)'; },2000);
    });
}

function downloadSerials() {
    const getText=id=>{ const el=document.getElementById(id); return el?(el.textContent||'-').trim():'-'; };
    const disk=getText('disk1Serial'), cpu=getText('cpuSerial'), bios=getText('biosSerial'), mobo=getText('mbSerial'), smbios=getText('smbiosUUID');
    const content=`Disk: ${disk}\nCpu: ${cpu}\nBios: ${bios}\nMobo: ${mobo}\nSmbios: ${smbios}\n`;
    const blob=new Blob([content],{ type:'text/plain;charset=utf-8' });
    const url=URL.createObjectURL(blob);
    const a=document.createElement('a');
    a.href=url; a.download='serials.txt'; document.body.appendChild(a); a.click(); document.body.removeChild(a); URL.revokeObjectURL(url);
}

function switchTab(tabName, el) {
    document.querySelectorAll('.tab-content').forEach(c=>c.classList.remove('active'));
    document.querySelectorAll('.tab-button').forEach(b=>b.classList.remove('active'));
    document.querySelectorAll('.nav-icon').forEach(i=>i.classList.remove('active'));
    const target=document.getElementById(tabName);
    if(target) target.classList.add('active');
    try { if(el && el.classList) el.classList.add('active'); else if(event && event.target && event.target.classList) event.target.classList.add('active'); } catch(e){}
    if(tabName==='serial'){
        fetch('/serials').then(r=>r.json()).then(data=>{
            const d1=data.disks&&data.disks[0], d2=data.disks&&data.disks[1];
            if(d1){ const m=document.getElementById('disk1Model'); if(m) m.textContent=d1.model||'-'; const s=document.getElementById('disk1Serial'); if(s) s.textContent=d1.serial||'-'; }
            if(d2){ const m=document.getElementById('disk2Model'); if(m) m.textContent=d2.model||'-'; const s=document.getElementById('disk2Serial'); if(s) s.textContent=d2.serial||'-'; }
            const cpu=document.getElementById('cpuSerial'); if(cpu) cpu.textContent=data.cpuSerial||'-';
            const bios=document.getElementById('biosSerial'); if(bios) bios.textContent=data.biosSerial||'-';
            const mb=document.getElementById('mbSerial'); if(mb) mb.textContent=data.motherboardSerial||'-';
            const uuid=document.getElementById('smbiosUUID'); if(uuid) uuid.textContent=data.smbiosUUID||'-';
        }).catch(err=>console.error('Serials error',err));
    }
}

document.addEventListener('DOMContentLoaded',function(){
    const first=document.querySelector('.sidebar .nav-icon');
    if(first) first.classList.add('active');
    if(typeof lucide!=='undefined'){ lucide.createIcons(); }
});

function closeApp() { if(confirm('Are you sure you want to close the application?')) window.close(); }
)html";

                    const std::string SCRIPT_PART3 = R"html(
</script>
</body>
</html>
)html";

                    // Final concatenated script
                    const std::string SCRIPT = SCRIPT_PART1 + SCRIPT_PART2 + SCRIPT_PART3;

    // Concatenate all parts to create the complete HTML
    const std::string CONTENT = HTML_HEAD + SCRIPT;
}