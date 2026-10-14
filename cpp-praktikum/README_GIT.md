# Git: Änderungen mit GitHub zusammenführen

Diese Anleitung hilft bei den Meldungen:

```text
fatal: Need to specify how to reconcile divergent branches.
! [rejected] main -> main (non-fast-forward)
```

## Was bedeutet das?

Auf deinem Computer und auf GitHub gibt es unterschiedliche neue Commits.
Ein Commit ist ein gespeicherter Stand deines Projekts. Git muss die beiden
Verläufe zuerst zusammenführen, bevor du wieder hochladen kannst.

Wir verwenden dafür einen **Merge**, also eine Zusammenführung.

## 1. Aktuellen Zustand ansehen

Öffne ein Terminal im Projektordner und gib ein:

```bash
git status
```

Danach wählst du den passenden Fall unten. Führe die Befehle einzeln aus.
Wenn ein Fehler oder `CONFLICT` erscheint, führe die weiteren Befehle zunächst
nicht aus. Die Zusammenführung muss dann erst geklärt werden.

## 2a. Keine offenen Dateiänderungen

Wenn Git `working tree clean` meldet:

```bash
git pull --no-rebase --no-edit
git push
```

- `pull` holt die Änderungen von GitHub.
- `--no-rebase` führt die beiden Verläufe mit einem Merge zusammen.
- `--no-edit` übernimmt die vorgeschlagene Commit-Nachricht ohne Editor.
- `push` lädt deine lokalen Commits auf GitHub hoch.

## 2b. Noch nicht committete Dateiänderungen

Wenn `git status` geänderte oder neue Dateien anzeigt, lege diese zuerst
vorübergehend beiseite:

```bash
git stash push -u -m "Zwischenstand vor Pull"
git pull --no-rebase --no-edit
git stash pop
git push
```

- `stash push` legt deine offenen Dateiänderungen vorübergehend ab.
- `-u` nimmt auch neue Dateien mit, die Git noch nicht verfolgt.
- `stash pop` holt diese Änderungen nach dem Merge zurück.

Benutze diese Variante nur, wenn tatsächlich offene Änderungen vorhanden sind.
Falls Git `No local changes to save` meldet, überspringe `git stash pop`, damit
du nicht versehentlich einen älteren Stash zurückholst.

## 3. Eigene Dateiänderungen ebenfalls hochladen

Die zurückgeholten Änderungen sind noch nicht committet. `git push` allein
lädt sie deshalb nicht hoch.

Prüfe zunächst mit `git status`, welche Dateien geändert wurden. Wenn alle
angezeigten Änderungen in deinen nächsten Commit gehören:

```bash
git add -A
git commit -m "Aufgaben aktualisiert"
git push
```

- `add -A` merkt alle neuen, geänderten und gelöschten Dateien für den Commit vor.
- `commit` speichert diesen Stand lokal mit einer kurzen Beschreibung.
- `push` überträgt den Commit auf GitHub.

## Bei einem Konflikt

Ein Konflikt bedeutet, dass Git Änderungen nicht automatisch zusammenführen
kann. Er kann beim `pull` oder beim Zurückholen mit `stash pop` auftreten.

Führe zunächst nur Folgendes aus:

```bash
git status
```

Die Ausgabe nennt die betroffenen Dateien. Schicke diese Ausgabe zusammen mit
der Fehlermeldung in den Chat, damit die Stellen gemeinsam geklärt werden können.
