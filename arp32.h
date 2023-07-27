#pragma once

template<uint32_t note_count, uint32_t ppqn>
class Arp32 {
public:
  Arp32() {
    Clear();
  }

  void NoteOn(uint32_t num, uint32_t vel) {
    NoteOff(num);

    //Look for a free slot.
    uint32_t slot = 1;
    while (slot < note_count + 1) {
      if (_notes[slot].num == kEmpty) break;
      slot++;
    }
    // If slot is beyound the buffer, i.e. there's no free slots,
    // take the least recent note and replace it with the new one.
    if (slot == note_count + 1) {
      slot = _input_order[0];
      _RemoveNote(slot);
    }

    // Search for the index of the first note that
    // is higher than the one being inserted.
    auto idx = _bottom_idx;
    while (_notes[idx].num < num) idx = _notes[idx].next;

    // If the note that is higher than inserted
    // was the bottom one, the inserted becomes the bottom.
    if (idx == _bottom_idx) _bottom_idx = slot;

    // Insert the note, i.e. asign attributes and
    // link next / prev.
    _notes[slot].num = num;
    _notes[slot].vel = vel;
    _notes[slot].next = idx;
    _notes[slot].prev = _notes[idx].prev;

    // Link previous/next notes
    _notes[_notes[idx].prev].next = slot;
    _notes[idx].prev = slot;

    // Insert the slot index to the end of the
    // input order array.
    _input_order[_size - 1] = slot;

    _size ++;
  }

  void NoteOff(uint32_t num) {
    // Serach for an index of the note that supposed to be off.
    // Here we're taking advantage of the fact that the 
    // container of the notes list is plain array so we can 
    // just for-loop through it.
    uint32_t idx = 0;
    for (uint32_t i = 0; i < _size; i++) {
      if (_notes[i].num == num) {
          idx = i;
          break;
      }
    }
    if (idx != 0) _RemoveNote(idx);
  }

  void Clear() {
    memset(_input_order, 0, sizeof(uint32_t) * note_count);
    _notes[0].num = kSentinel;
    _notes[0].next = 0;
    _notes[0].prev = 0;
    _size = 1;
    for (uint32_t i = _size; i < note_count + 1; i++) {
        _notes[i].num = kEmpty;
        _notes[i].next = kUnlinked;
        _notes[i].prev = kUnlinked;
    }
  }

private:
  void _RemoveNote(uint32_t idx) {
    // Send note off
    //_on_note_off(_notes[idx].num);
    // Link next/previous notes to each other,
    // excluding the removed note from the chain.
    _notes[_notes[idx].prev].next = _notes[idx].next;
    _notes[_notes[idx].next].prev = _notes[idx].prev;
    if (idx == _bottom_idx) _bottom_idx = _notes[idx].next;

    // "Remove" note, i.e. mark empty
    _notes[idx].num = kEmpty;
    _notes[idx].next = kUnlinked;
    _notes[idx].prev = kUnlinked;

    // Remove the note from the input_order and 
    // rearrange the latter.
    for (uint32_t i = 0; i < _size; i++) {
        if (_input_order[i] == idx) {
            while (i < _size) {
                _input_order[i] = _input_order[i + 1];
                i++;
            }
        }
    }
    _size--;
  }

  struct Note {
    uint32_t num;
    uint32_t vel;
    uint32_t next;
    uint32_t prev;
  };

  static const uint32_t kSentinel = 0xff;
  static const uint32_t kEmpty    = 0xfe;
  static const uint32_t kUnlinked = 0xfd;

  Note _notes[note_count + 1];
  uint32_t _input_order[note_count];
  
  uint32_t _bottom_idx     = 0;
  uint32_t _size           = 0;
};
