ExoBrowser JS API Specification
===============================

The API is enterely async as actual commands are run on the `BrowserThread::UI`
so that the nodeJS thread do not block on possible lenghty UI operations.

Native objects live on the BrowserThread::UI thread but are aware of their
JS counterparts Wrappers.

#### API v0.6-alpha:

```
var s = api.exo_session({
  path: '',
  off_the_record: false
  cookie_handlers: { ... }
});

exo_session.set_cookie_handlers({
  load_for_key: null,             /* load_for_key(key, cb_(cookies)); */
  flush: null,                    /* flush(cb_()); */
  add: null,                      /* add(cc); */
  remove: null,                   /* remove(cc); */
  update_access_time: null,       /* update_access_time(cc); */
  force_keep_session_state: null  /* force_keep_session_state(); */
});

exo_session.add_visited_link(url, [cb_]);
exo_session.clear_visited_links([cb_]);

exo_session.off_the_record();
exo_session.path();


var b = api.exo_browser({
  size: [123, 23],
  icon_path: ''
});

exo_browser.ready();
exo_browser#ready()

exo_browser#open_url(url)
exo_browser#resize()
exo_browser#kill()

exo_browser#frame_close(frame)
exo_browser#frame_keyboard(frame, event)
exo_browser#frame_navigation_state(frame, state)
exo_browser#frame_load_fail(frame, url, error_code, error_desc)
exo_browser#frame_load_finish(frame, url)
exo_browser#frame_loading_start(frame)
exo_browser#frame_loading_stop(frame)
exo_browser#frame_created(frame, dispostion, initial_pos, from)
exo_browser#frame_find_reply(frame, rid, matches, selection, active, final)

exo_browser.frame(name);

exo_browser.frames();
exo_browser.controls();
exo_browser.pages();

exo_browser.set_control(type, frame, [cb_]);
exo_browser.unset_control(type, [cb_]);
exo_browser.set_control_dimension(type, size, [cb_]);

exo_browser.show_floating(frame, x, y, width, height, [cb_]);
exo_browser.hide_floating([cb_]);

exo_browser.add_page(frame, [cb_]);
exo_browser.remove_page(frame, [cb_]);
exo_browser.clear_page(frame, [cb_]);

exo_browser.focus([cb_]);
exo_browser.maximize([cb_]);
exo_browser.set_title(title, [cb_]);


var f = api.exo_frame({
  name: '',
  url: '',
  session: s
});

exo_frame.set_context_menu_handler(function(params, cb_) {
  return cb_(null, {
    'Foo': function() { ... },
    '': null,
    'Bar': function() { ... }
  });
});

exo_frame.ready();
exo_frame#ready();

exo_frame.url();
exo_frame.name();
exo_frame.visible();
exo_frame.ready();
exo_frame.parent();
exo_frame.type();
exo_frame.loading();
exo_frame.title();

exo_frame.find(text, forward, case, next, [cb_]);
exo_frame.find_stop(action, [cb_]);

exo_frame.capture(cb_);
exo_frame.zoom(zoom, [cb_]);

exo_frame.load_url(url, [cb_]);
exo_frame.go_back_or_forward(offset, [cb_]);
exo_frame.reload([cb_]);
exo_frame.stop([cb_]);
exo_frame.focus([cb_]);

exo_frame.undo([cb_]);
exo_frame.redo([cb_]);
exo_frame.cut_selection([cb_]);
exo_frame.copy_selection([cb_]);
exo_frame.paste([cb_]);
exo_frame.delete_selection([cb_]);
exo_frame.select_all([cb_]);
exo_frame.unselect([cb_]);

```


#### Internal API v0.6-alpha:

```
/*********************************************************************/
/* EXOSESSION */
/*********************************************************************/
_exo_browser._createExoSession({
  path: '~/.config/smthg',
  off_the_record: false
}, cb_);

s._setCookiesLoadForKeyHandler(hdlr_);
s._setCookiesFlushHandler(hdlr_);

s._setCookiesAddCallback(cb_);
s._setCookiesDeleteCallback(cb_);
s._setCookiesUpdateAccessTimeCallback(cb_);
s._setCookiesForceKeepSessionStateCallback(cb_);

s._addVisitedLink(url, cb_);
s._clearVisitedLinks(cb_);

s._clearAllData(cb_);
s._clearDataForOrigin(url, cb_);

/*********************************************************************/
/* EXOBROWSER */
/*********************************************************************/
_exo_browser._createExoBrowser({
  size: [123, 23],
}, cb_);

b._size(cb_);
b._position(cb_);
b._focus(cb_);
b._maximize(cb_);
b._setTitle(title, cb_);

b._setOpenURLCallback(cb_);
b._setResizeCallback(cb_);
b._setKillCallback(cb_);

b._setFrameCloseCallback(cb_);
b._setFrameCreatedCallback(cb_);
b._setFrameKeyboardCallback(cb_);
b._setNavigationStateCallback(cb_);
b._setFindReplyCallback(cb_);

b.kill(cb_);

/*********************************************************************/
/* EXOFRAME */
/*********************************************************************/
_exo_browser._createExoFrame({
  name: '',
  url: '',
  session: s,
}, cb_);

f._loadURL(url, cb_);
f._goBackOrForward(offset, cb_);
f._reload(cb_);
f._stop(cb_);
f._undo(cb_);
f._redo(cb_);
f._cutSelection(cb_);
f._copySelection(cb_);
f._paste(cb_);
f._deleteSelection(cb_);
f._selectAll(cb_);
f._unselect(cb_);
f._focus(cb_);
f._find(rid, text, forward, sensitive, next, cb_)
f._stopFinding(action, cb_);
f._capture(cb_);
f._getDevToolsFrame(cb_);
f._zoom(zoom, cb_);
f._zoomLevel(cb_);

f._name(cb_);
f._type(cb_);

f._setBuildContextMenuHandler(cb_);

f._setLoadFailCallback(cb_);
f._setLoadFinishCallback(cb_);
f._setLoadingStartCallback(cb_);
f._setLoadingStopCallback(cb_);

/* CONTROL */

b._setControl(type, frame, cb_);
b._unsetControl(type, cb_);
b._setControlDimension(type, size, cb_);

/* FLOATING */

b._showFloating(frame, x, y, width, height, cb_);
b._hideFloating(cb_);

/* PAGE */

b._showPage(name, cb_);
b._removePage(name, cb_);
b._clearPage(name, cb_);
```

