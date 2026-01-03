var isRunning = false;
var isResetting = true;
var animationId = null;
var inGround = false;
var stats = {
    x : 5,
    y : 5,
    wid : 20,
    hei : 20,
    velX : 1,
    velY : 0.5,
    accelX : 0,
    accelY : 0.2,
    damping : 0.95,
    minVel : 1.5,
    color : "#222222",
};
var settings = ["x", "y", "wid", "hei", "velX", "velY", "accelX", "accelY", "damping"];
var copyStats, canvas, ctx, maxwid, maxhei;
var timeSpeed = 1;

function getData(){
    canvas = document.getElementById("canvas");
    ctx = canvas.getContext("2d");
    maxwid = canvas.width;
    maxhei = canvas.height;
    for(var i=0; i<9; i++){
        var localSetting = localStorage.getItem(`physics-${settings[i]}`);
        if(localSetting!=null){
            stats[settings[i]] = Number(localSetting);
            document.getElementById(`${settings[i]}`).value = localSetting;
        }
    }
    document.getElementById("timeSpeed").value = localStorage.getItem("timeSpeed")||1;
    timeSpeed = localStorage.getItem("timeSpeed")||1;
    copyStats = { ...stats };//获取参数
    drawFrame();
    returnFeedback()
}

function setData(){
    for(var i=0; i<9; i++){
        var nowSetting = Number(document.getElementById(`${settings[i]}`).value);
        if(nowSetting!=null){
            localStorage.setItem(`physics-${settings[i]}`, nowSetting);
            stats[settings[i]] = nowSetting;
        }
    }
    copyStats = { ...stats };//修改参数
    drawFrame();
    returnFeedback();
}

function draw(){
    if(!isRunning)return;
    copyStats.x += copyStats.velX*timeSpeed;
    copyStats.y += copyStats.velY*timeSpeed;
    copyStats.velX += copyStats.accelX*timeSpeed;
    copyStats.velY += copyStats.accelY*timeSpeed;
    if(copyStats.x+copyStats.wid>=maxwid){
        copyStats.x = maxwid-copyStats.wid;
        copyStats.velX *= -1*copyStats.damping;
    }
    if(copyStats.x<=0){
        copyStats.x = 0;
        copyStats.velX *= -1*copyStats.damping;
    }
    if(copyStats.y+copyStats.hei>=maxhei){
        copyStats.y = maxhei-copyStats.hei;
        if(Math.abs(copyStats.velY)<=Math.abs(copyStats.minVel)){
            inGround = true;
        }
        copyStats.velY = inGround ? copyStats.accelY : copyStats.velY*-1*copyStats.damping;
    }
    if(copyStats.y<=0){
        copyStats.y = 0;
        copyStats.velY *= -1*copyStats.damping;
    }
    for(var i=0; i<9; i++){
        if([0, 1, 4, 5].includes(i)){
            copyStats[settings[i]] = Math.round(copyStats[settings[i]]*100000)/100000;//保留小数
        }
    }
    drawFrame();
    returnFeedback();
    animationId = requestAnimationFrame(draw);
}

function drawFrame(){
    ctx.clearRect(0, 0, maxwid, maxhei);
    ctx.fillStyle = copyStats.color;
    ctx.fillRect(copyStats.x, copyStats.y, copyStats.wid, copyStats.hei);
}

function startOrEndAnimation(){
    if(isResetting){
        isRunning = true;
        isResetting = false;
        inGround = false;
        document.getElementById("stop").disabled = false;
        for(var i=0; i<9; i++){
            document.getElementById(`${settings[i]}`).disabled = true;
        }
        animationId = requestAnimationFrame(draw);
    }else{
        isRunning = false;
        isResetting = true;
        cancelAnimationFrame(animationId);
        animationId = null;
        document.getElementById("stop").disabled = true;
        for(var i=0; i<9; i++){
            document.getElementById(`${settings[i]}`).disabled = false;
        }
        copyStats = { ...stats };
        drawFrame();
        returnFeedback();
    }
}

function stopAnimation(){
    if(isResetting)return;
    if(isRunning){
        isRunning = false;
        cancelAnimationFrame(animationId);
    }else{
        isRunning = true;
        animationId = requestAnimationFrame(draw);
    }
}

function returnFeedback(){
    for(var i=0; i<9; i++){
        if([0, 1, 4, 5].includes(i)){
            document.getElementById(`feedback${settings[i].charAt(0).toUpperCase()
                +settings[i].slice(1)}`).innerHTML = copyStats[settings[i]];
        }
    }
}

function changeTimeSpeed(){
    var newTimeSpeed = document.getElementById("timeSpeed").value;
    if(newTimeSpeed==0)return;
    timeSpeed = newTimeSpeed;
    localStorage.setItem("timeSpeed", timeSpeed);
}