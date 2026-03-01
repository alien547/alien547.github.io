const WORKER_URL = 'https://alien547.us.ci';

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

    // 格式化日期时间
    const dateStr = new Date(msg.timestamp).toLocaleString('zh-CN', {
        year: 'numeric',
        month: '2-digit',
        day: '2-digit',
        hour: '2-digit',
        minute: '2-digit',
        second: '2-digit'
    });

    // 显示哈希标识（如果存在）
    const hashDisplay = msg.hash_id ? ` [ID：${msg.hash_id}]` : '';

    div.innerHTML = `<strong>${msg.user || '匿名'}${hashDisplay}</strong> ${dateStr}<br>${msg.text}`;
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