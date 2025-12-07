function updateTime(){
    var language = localStorage.getItem("language");
    var now = new Date();
    var year = now.getFullYear();
    var month = now.getMonth() + 1;
    var day = now.getDate();
    var hour = now.getHours().toString().padStart(2, '0');
    var minute = now.getMinutes().toString().padStart(2, '0');
    var second = now.getSeconds().toString().padStart(2, '0');//自动补0
    var formattedTime = language=="zh-CN" ? `${year}年 ${month}月 ${day}日 ${hour}:${minute}:${second}` : 
    `Year${year} Month${month} Day${day} ${hour}:${minute}:${second}`;
    document.getElementById('time').innerHTML = formattedTime;
}

setInterval(updateTime, 1000);//每秒刷新