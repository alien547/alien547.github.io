language = localStorage.getItem("language") || "zh-CN";
nowIn = localStorage.getItem("nowIn") || "home";
window.location.replace(`${language}/${nowIn}.html`);//导向