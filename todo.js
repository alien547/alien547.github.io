function setTodo(){
    var todo = document.getElementById("todo").value;
    var key = localStorage.getItem("nowTodo");
    if(key=="newTodo"){
        if(todo.trim()==""){
            document.getElementById("todo").value = "";//空输入
            return;
        }
        var todos = Object.keys(localStorage).filter(key => key.startsWith("todo"));
        var index = todos.length>0 ? Number(todos[todos.length-1].substring(4))+1 : 1;//index为当前todo列表中数值最大的todo加1或为1
        localStorage.setItem("nowTodo", `todo${index}`);
        localStorage.setItem(`todo${index}`, todo);
        var todoList = document.createElement("li");//创建新的按钮
        todoList.innerHTML = `
        <button id="todo${index}" class="todos" onclick="getTodo('todo${index}')">${todo}</button>
        `
        document.getElementById("change").innerHTML = "×";
        document.getElementById("todoList").appendChild(todoList);
    }else{
        if(document.getElementById("change").innerHTML=="√"){
            if(todo==""){
                document.getElementById(key).remove();//输入为空则删除
                document.getElementById("todo").placeholder = "新的待办";
                localStorage.removeItem(key);
                localStorage.setItem("nowTodo", "newTodo");
            }else{
                document.getElementById(key).innerHTML = todo;//修改按钮信息
                document.getElementById("change").innerHTML = "×";
                localStorage.setItem(key, todo);
            }
        }else{
            document.getElementById(key).remove();
            document.getElementById("todo").value = "";
            document.getElementById("change").innerHTML = "√";
            localStorage.removeItem(key);
            localStorage.setItem("nowTodo", "newTodo");
        }
    }
}

function getTodo(key){
    localStorage.setItem("nowTodo", key);
    var todo = localStorage.getItem(key);
    document.getElementById("todo").value = todo;
    document.getElementById("change").innerHTML = "×";
}

function getTodos(){
    localStorage.setItem("nowTodo", "newTodo");
    var IStodoKeys = Object.keys(localStorage).filter(key => key.startsWith("todo"));
    IStodoKeys.sort((a, b) => {
        var a = Number(a.substring(4)) || 0;
        var b = Number(b.substring(4)) || 0;
        return a - b;//递增排序
    })
    IStodoKeys.forEach((key, index) => {
        var todo = localStorage.getItem(key);
        var todoList = document.createElement("li");//生成todo列表
        todoList.innerHTML = `
        <button id="${key}" class="todos" onclick="getTodo('${key}')">${todo}</button>
        `
        document.getElementById("todoList").appendChild(todoList);
    })
}

function changeTodo(){
    var todo = document.getElementById("todo").value;
    var key = localStorage.getItem("nowTodo");
    var IStodo = localStorage.getItem(key);
    if(todo.trim()==""){
        document.getElementById("todo").value = "";
        if(key!="newTodo"){
            document.getElementById("todo").placeholder = "删除？";
        }
    }
    document.getElementById("change").innerHTML = todo==IStodo ? "×" : "√";//文本不变则显示×
}

function addTodo(){
    localStorage.setItem("nowTodo", "newTodo");
    document.getElementById("todo").value = "";
}