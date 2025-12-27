function changeLanguage(){
    var onLanguage = document.getElementById("language");
    var language = onLanguage.options[onLanguage.selectedIndex].value;
    localStorage.setItem("language", language);
    localStorage.setItem("changeQuote", false);
    window.location.replace(`../index.html`);
}