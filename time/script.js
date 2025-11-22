function updateTime(){
    var now = new Date();
    var year = now.getFullYear();
    var month = now.getMonth() + 1;
    var day = now.getDate();
    var hour = now.getHours();
    var minute = now.getMinutes();
    var second = now.getSeconds();
    var formattedTime = `${year}-${month}-${day} ${hour}:${minute}:${second}`;
    document.getElementById('time').innerHTML = formattedTime;
}

setInterval(updateTime, 1000);