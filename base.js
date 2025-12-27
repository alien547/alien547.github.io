function changeLanguage(){
    var onLanguage = document.getElementById("language");
    var language = onLanguage.options[onLanguage.selectedIndex].value;
    localStorage.setItem("language", language);
    localStorage.setItem("changeQuote", false);
    window.location.replace(`../index.html`);
}

function getLanguage(language){
    if(localStorage.getItem("language")==null){
        localStorage.setItem("language", language);
    }
}