function setTodo(){
    var todo = document.getElementById("todo").value.trim();
    if(document.getElementById("change").innerHTML=="√"){
        if(todo!=""){
            localStorage.setItem("todo", todo);
            document.getElementById("todo").setAttribute("readonly", false);
            document.getElementById("change").innerHTML = "×";
        }
    }else{
        localStorage.removeItem("todo");
        document.getElementById("todo").value = "";
        document.getElementById("todo").setAttribute("readonly", true);
        document.getElementById("change").innerHTML = "√";
    }
}

function getTodo(){
    var todo = localStorage.getItem("todo");
    if(todo!=null){
        document.getElementById("todo").value = todo;
        document.getElementById("change").innerHTML = "×";
    }
}

function deleteSpace(){
    var todo = document.getElementById("todo").value.trim();
    if(todo==""){
        document.getElementById("todo").value = "";
    }
}