CC = gcc
OBJS = utilities.o realtor.o offer.o customer.o yad3.o yad3_main.o
ADTS = customer.o utilities.o realtor.o offer.o
EXEC = mtm_yad3
DEBUG = -g -DNDEBUG
CFLAGS = -std=c99 -Wall -I. -Werror -pedantic-errors $(DEBUG)
LIBS = -L. -lmtm -lex1
TESTS = offer_test customer_test realtor_test yad3_test list_mtm_test
O_FILES = $(OBJS) $(LIST_PATH)list_mtm.o
TEST_O_FILES = $(TEST_PATH)*.o
TEST_PATH=./tests/
LIST_PATH=./list_mtm/
TEST_GCC = $(CC) $(CFLAGS) -c $(LIBS) $(TEST_PATH)$*.c -o $(TEST_PATH)$@

$(EXEC) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $@

tests : $(TESTS)

customer.o : customer.c customer.h utilities.h mtm_ex2.h \
 map.h set.h yad3.h realtor.h apartment_service.h apartment.h list.h \
 offer.h
	$(CC) $(CFLAGS) -c $(LIBS) $*.c -o $@

offer.o : offer.c offer.h utilities.h mtm_ex2.h map.h \
 set.h yad3.h realtor.h apartment_service.h apartment.h customer.h list.h
	$(CC) $(CFLAGS) -c $(LIBS) $*.c -o $@

realtor.o : realtor.c realtor.h utilities.h mtm_ex2.h \
 map.h set.h yad3.h customer.h apartment_service.h apartment.h list.h \
 offer.h
	$(CC) $(CFLAGS) -c $(LIBS) $*.c -o $@

utilities.o : utilities.c utilities.h mtm_ex2.h map.h \
 set.h yad3.h realtor.h apartment_service.h apartment.h customer.h list.h \
 offer.h
	$(CC) $(CFLAGS) -c $(LIBS) $*.c -o $@

yad3.o : yad3.c yad3.h utilities.h mtm_ex2.h map.h set.h \
 realtor.h apartment_service.h apartment.h customer.h list.h offer.h
	$(CC) $(CFLAGS) -c $(LIBS) $*.c -o $@

yad3_main.o : yad3_main.c utilities.h mtm_ex2.h map.h \
 set.h yad3.h realtor.h apartment_service.h apartment.h customer.h list.h \
 offer.h
	$(CC) $(CFLAGS) -c $(LIBS) $*.c -o $@

list_mtm.o : $(LIST_PATH)list_mtm.c $(LIST_PATH)list_mtm.h
	$(CC) $(CFLAGS) -c $(LIBS) $(LIST_PATH)$*.c -o $(LIST_PATH)$@

offer_test : $(TEST_PATH)offer_test.o $(ADTS)
	$(CC) $(TEST_PATH)$@.o $(ADTS) $(CFLAGS) $(LIBS) -o $@

customer_test : $(TEST_PATH)customer_test.o $(ADTS)
	$(CC) $(TEST_PATH)$@.o $(ADTS) $(CFLAGS) $(LIBS) -o $@

realtor_test : $(TEST_PATH)realtor_test.o $(ADTS)
	$(CC) $(TEST_PATH)$@.o $(ADTS) $(CFLAGS) $(LIBS) -o $@

yad3_test : $(TEST_PATH)yad3_test.o yad3.o $(ADTS)
	$(CC) $(TEST_PATH)$@.o yad3.o $(ADTS) $(CFLAGS) $(LIBS) -o $@

list_mtm_test : $(LIST_PATH)list_mtm.o $(TEST_PATH)list_mtm_test.o
	$(CC) $(TEST_PATH)$@.o $(LIST_PATH)list_mtm.o $(CFLAGS) $(LIBS) -o $@

offer_test.o : $(TEST_PATH)offer_test.c offer.h utilities.h $(TEST_PATH)test_utilities.h mtm_ex2.h \
 map.h set.h yad3.h realtor.h apartment_service.h apartment.h customer.h \
 list.h
	$(TEST_GCC)

customer_test.o : $(TEST_PATH)customer_test.c $(TEST_PATH)test_utilities.h customer.h utilities.h \
 mtm_ex2.h map.h set.h yad3.h realtor.h apartment_service.h apartment.h \
 list.h offer.h
	$(TEST_GCC)

realtor_test.o : $(TEST_PATH)realtor_test.c utilities.h $(TEST_PATH)test_utilities.h mtm_ex2.h \
 map.h set.h yad3.h realtor.h apartment_service.h apartment.h customer.h \
 list.h offer.h
	$(TEST_GCC)

yad3_test.o : $(TEST_PATH)yad3_test.c utilities.h $(TEST_PATH)test_utilities.h mtm_ex2.h map.h \
 set.h yad3.h realtor.h apartment_service.h apartment.h customer.h list.h \
 offer.h
	$(TEST_GCC)

list_mtm_test.o : $(TEST_PATH)list_mtm_test.c $(TEST_PATH)test_utilities.h $(LIST_PATH)list_mtm.h
	$(TEST_GCC)

clean:
	rm -f $(TESTS) $(O_FILES) $(TEST_O_FILES) $(EXEC)