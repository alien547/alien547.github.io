const WORKER_URL = 'https://api.alien547.us.ci';

function getInfo(){
    username = localStorage.getItem("username");
    message = localStorage.getItem("message");
    document.getElementById("username").value = username;
    document.getElementById("message").value = message;
}

function setInfo(){
    username = document.getElementById("username").value;
    message = document.getElementById("message").value;
    localStorage.setItem("username", username);
    localStorage.setItem("message", message);
}

function displayMessage(msg) {
    const div = document.createElement('div');
    div.style.margin = '5px 0';
    // 如果存在 ip 字段，显示在用户名旁边（例如：用户名 (IP: xxx.xxx.xxx.xxx)）
    const ipDisplay = msg.ip ? ` (IP: ${msg.ip})` : '';
    div.innerHTML = `<strong>${msg.user || '匿名'}${ipDisplay}</strong> ${new Date(msg.timestamp).toLocaleTimeString()}<br>${msg.text}`;
    document.getElementById('messages').appendChild(div);
}

async function pollMessages(){
    try{
        const res = await fetch(`${WORKER_URL}/api/messages`);
        const messages = await res.json();
        const container = document.getElementById('messages');
        container.innerHTML = '';
        messages.forEach(displayMessage);
    }catch(e){
        console.error('拉取失败', e);
    }
}

async function sendMessage(){
    try{
        const user = document.getElementById('username').value.trim() || '匿名';
        const text = document.getElementById('message').value.trim();
        if(!text)return;
        await fetch(`${WORKER_URL}/api/messages`, {
            method: 'POST',
            headers: {'Content-Type': 'application/json'},
            body: JSON.stringify({ user, text }),
        });
        document.getElementById('message').value = '';
        pollMessages();
    }catch(e){
        console.error('发送失败', e);
        alert('发送失败，请稍后重试');
    }
}

setInterval(pollMessages, 10000);
window.onload = () => {
    getInfo();
    pollMessages();
    document.getElementById('send').onclick = sendMessage;
    document.getElementById('message').onkeypress = (e) => {
        if(e.key==='Enter')sendMessage();
    };
};